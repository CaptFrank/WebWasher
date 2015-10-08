/*
 * system.cpp
 *
 *  Created on: Jul 4, 2015
 *      Author: francis-ccs
 */

#include <service/services/system/system.h>
#include <service/services/coms/coms.h>
/**
 * @brief The default constructor for the class.
 *
 * By default, both the system caches (i.e. hearbeat and the status
 * caches) are added the linked list of caches internally. This is done
 * to allow the BIOS to function fully without having to register those
 * caches manually.
 *
 * @param sensors	 	The sensors taht are in use
 */
void system_srv::BIOS_setup(sensor_t* sensors[]){

	/*
	 * Setup the heartbeat cache
	 */
	system_t::heartbeat_cache.fxn.f_ptr 	= system_t::update_heartbeat_cache;
	system_t::heartbeat_cache.node			= &system_t::heart;
	system_t::heartbeat_cache.type 			= CACHE_TYPE_HEARTBEAT;
	system_t::heartbeat_cache.msg			= MSG_TYPE_HEARTBEAT;
	system_t::heartbeat_cache.next			= &system_t::status_cache;
	system_t::heartbeat_cache.prev			= NULL;
	system_t::heartbeat_cache.sensor		= NULL; 	// No sensor

	/*
	 * Setup the status cache
	 */
	system_t::status_cache.fxn.f_ptr 		= system_t::update_status_cache;
	system_t::status_cache.node				= &system_t::status;
	system_t::status_cache.type 			= CACHE_TYPE_STATUS;
	system_t::status_cache.msg				= MSG_TYPE_STATUS;
	system_t::status_cache.next				= NULL;
	system_t::status_cache.prev				= &system_t::heartbeat_cache;
	system_t::status_cache.sensor			= NULL; 	// No sensor

	/*
	 * Create the list
	 */
	list = &heartbeat_cache;

	/*
	 * Boot the scheduler
	 */
	system_t::scheduler = new scheduler_t();
	suspend_queue = system_t::scheduler->suspend_queue;

	/*
	 * Add the sensors
	 */

	// copy over the sensor pointers.
	memcpy((void*)system_t::sensors, (void*)sensors, NUMBER_OF_SENSORS);
	NOTIFY_INFO("System BIOS Booted.");
	digitalWrite(STATUS_LED, HIGH);
}

/**
 * @brief Reboots a component
 *
 * @param type			The component to reboot
 */
void system_srv::BIOS_reboot(bios_reboot_t type){

	/*
	 * Iface
	 */
	extern coms_t* coms;

	/*
	 * Update the state
	 */
	state = SYS_STATE_REBOOT;
	digitalWrite(STATUS_LED, LOW);
	NOTIFY_INFO("Rebooting " + String(type));

	/*
	 * Switch on type
	 */
	switch(type){

	case BIOS_REBOOT_COMS:

		/*
		 * Delete the current coms and re init it.
		 */
		coms->disconnect(INTERFACE_BOTH);
		delete coms;

		/*
		 * Reconnect
		 */
		coms = new coms_t();
		coms->connect(INTERFACE_BOTH);
		break;

	case BIOS_REBOOT_MQTT:

		/*
		 * Reboot the mqtt interface
		 */
		coms->mqtt_if->disconnect();
		delete coms->mqtt_if;

		/*
		 * Rebuild the interface
		 */
		coms->mqtt_if = \
				new mqtt_t(coms->ip_stack);
		coms->connect(INTERFACE_MQTT);
		break;

	case BIOS_REBOOT_WIFI:

		/*
		 * Reboot the wifi engine
		 */
		coms->wifi_if->disconnect();
		delete coms->wifi_if;

		/*
		 * Rebuild the interface
		 */
		coms->wifi_if = 					\
				new wifi_t(coms->ip_stack, \
						   &coms->wifi_attr);
		coms->wifi_if->connect(WIFI_SSID, WIFI_PASS);
		break;

	case BIOS_REBOOT_OS:

		/*
		 * HAng the OS and reboot manually
		 */
		BIOS_hang();
		break;

	case BIOS_REBOOT_SYS_HARD:
	case BIOS_REBOOT_SYS_SOFT:

		// Use the reset vector
		PRCMMCUReset(true);
		break;

	default:
		break;
	}
}

/**
 * @brief Alerts the system
 *
 * @param alert			The alert type
 */
void system_srv::BIOS_alert(bios_alerts_t alert){

	/*
	 * Container
	 */
	char* type = NULL;

	/*
	 * Switch to error state
	 */
	state = SYS_STATE_ERROR;
	digitalWrite(STATUS_LED, LOW);

	/*
	 * Switch on alert type
	 */
	switch(alert){
	case BIOS_ALERT_COMS_FAIL:
		type = "Coms Fail.";
		break;

	case BIOS_ALERT_PROCESSING_FAIL:
		type = "Process Fail.";
		break;

	case BIOS_ALERT_REBOOT:
		type = "Rebooting...";
		break;

	case BIOS_ALERT_REGISTER_FAIL:
		type = "Registered Failed.";
		break;

	case BIOS_ALERT_SENSOR_FAIL:
		type = "Sensor Fail.";
		break;

	case BIOS_ALERT_SYSTEM_FAIL:
		type = "System Fail.";
		break;

	case BIOS_ALERT_UPDATE_FAIL:
		type = "Update Fail";
		break;

	case BIOS_ALERT_SCHEDULER_FAIL:
		type = "Scheduler Fail.";
		break;

	case BIOS_ALERT_TASK_FAIL:
		type = "Task Fail.";
		break;

	default:
		break;
	}

	/*
	 * If we have text
	 */
	if(type){
		NOTIFY_ERROR(type);
	}
	return;
}

/*
 * Caching
 */

/**
 * @brief Registers a sensor cache and maps it to the system
 *
 * @param sensor		The sensor to map the cache from
 */
void system_srv::BIOS_register(sensor_t* sensor){

	// Containers
	cache_list_t* 	temp 			= system_t::list;
	cache_list_t* 	entry 			= (cache_list_t*)malloc(sizeof(cache_list_t));

	/*
	 * Go to the end of the list and check to see if there
	 * is already a mapped cache of the sort.
	 */
	while((temp->next != NULL)){

		/* Look for duplicates */
		if((temp->type != sensor->type)){

			/*
			 * Error we have a duplicate...
			 * We hang the OS and ask the user to reboot.
			 */
			system_t::BIOS_alert(BIOS_ALERT_REGISTER_FAIL);
			system_t::BIOS_reboot(BIOS_REBOOT_OS);
			break;
		}

		/* No duplicate */
		temp = temp->next;
	}

	/*
	 * Add the cache to the list
	 */
	entry->node 		= sensor->cache;
	entry->msg  		= sensor->msg;
	entry->next 		= NULL;
	entry->fxn.m_ptr  	= sensor::update;
	entry->prev 		= temp;
	entry->type 		= (cache_t)sensor->cache_type;
	entry->sensor		= sensor;
	temp->next 			= entry;
}

/**
 * @brief Update the caches
 *
 * @param type			The cache to update, by default set to all
 */
void system_srv::BIOS_update(cache_t type = CACHE_TYPE_ALL){

	// Containers
	bool 			rc;
	cache_list_t* 	temp 			= system_t::list;

	/*
	 * We update the caches
	 */
	while(temp->next != NULL){

		/*
		 * Update all
		 */
		if(type == CACHE_TYPE_ALL){

			/*
			 * Update
			 */
			if((temp->type != CACHE_TYPE_HEARTBEAT) ||
					(temp->type != CACHE_TYPE_STATUS)){


				/*
				 * We need to update the caches that require a sensor handle
				 */
				temp->fxn.m_ptr(temp->sensor);
				temp = temp->next;
				rc = true;

			}else if (temp->fxn.f_ptr()){

				/*
				 * OS caches that do not need sensor handle
				 */
				temp = temp->next;
				rc = true;
			}
			else{
				NOTIFY_ERROR("update failed: " \
						+ String(type));
				rc = false;
			}
		}else{

			/*
			 * Look for the type to update
			 */
			if((type != CACHE_TYPE_HEARTBEAT) ||
					(type != CACHE_TYPE_STATUS)){

				/*
				 * We need to update the caches that require a sensor handle
				 */
				temp = (system_t::BIOS_cache(type));
				rc = temp->fxn.m_ptr(temp->sensor);

			}else{

				/*
				 * OS caches that do not need sensor handle
				 */
				rc = (system_t::BIOS_cache(type))->fxn.f_ptr();
			}
		}

		/*
		 * Check for success
		 */
		if(!rc){

			/*
			 * Problem with the update
			 */
			system_t::BIOS_alert(BIOS_ALERT_UPDATE_FAIL);
			system_t::BIOS_reboot(BIOS_REBOOT_OS);
			break;
		}
	}
}

/*
 * Running
 */

/**
 * @brief Boots the systems
 *
 * @param tasks			The tasks to boot and add to the execution pool.
 */
void system_srv::BIOS_boot(task_t* tasks[]){

	/*
	 * Loop the task pointers and add them to our scheduler
	 */
	for(uint8_t i = 0; i < TASK_NUMBER; i ++){

		NOTIFY_INFO("Adding task: " + String(tasks[i]->_id));
		system_t::scheduler->add_task(tasks[i]);
	}
}

/**
 * @brief Runs the BIOS
 */
void system_srv::BIOS_run(){

	/*
	 * We run the scheduled tasks
	 */
	NOTIFY_INFO("Running the scheduler.");
	BIOS_state(SYS_STATE_ACTIVE);
	system_t::scheduler->run();
}

/**
 * @brief Suspends a task
 *
 * @param id			The task id to suspend
 */
void system_srv::BIOS_suspend(thread_id_t id){

	/*
	 * We get the thread id
	 */
	NOTIFY_INFO("Suspending thread id: " \
			+ String(id));
	system_t::suspend_queue->push(id);
}

/**
 * @brief Gets the cache of type specified.
 *
 * @param type			The type of cache to get
 * @return cache		The cache returned
 */
cache_list_t* system_srv::BIOS_cache(cache_t type){

	// Container
	cache_list_t* 	temp 			= system_t::list;

	/*
	 * Switch on the cache type
	 */
	switch(type){

		/*
		 * Return the hearbeat cache
		 */
		case CACHE_TYPE_HEARTBEAT:
		case CACHE_TYPE_STATUS:
		case CACHE_TYPE_TEMP_DATA:
		case CACHE_TYPE_ACC_DATA:

			// Find the cache
			while(temp){
				if(type == temp->type){

					NOTIFY_INFO("Cache type found: " \
							+ String(type));
					return temp;
				}
				temp = temp->next;
			}

		case CACHE_TYPE_ALL:
		default:
			NOTIFY_ERROR("Invalid cache index: " \
					+ String(type));
			return NULL;
	}
}


/**
 * @brief Sets the new state of the system
 *
 * @param state			The state
 */
void system_srv::BIOS_state(sys_state_t state){

	/*
	 * Set the state
	 */
	system_t::state = state;
}

/*
 * Private
 */

/**
 * @brief Updates the heartbeat cache
 */
bool system_srv::update_heartbeat_cache(){

	/*
	 * Update the heartbeat
	 */
	if((state != SYS_STATE_REBOOT) ||
			(state != SYS_STATE_ERROR)){
		system_t::heart.heartbeat_data.alive = true;
	}else{
		system_t::heart.heartbeat_data.alive = false;
	}

	system_t::heart.heartbeat_data.state = state;
	return true;
}

/**
 * @brief Updates the status cache
 */
bool system_srv::update_status_cache(){

	/*
	 * Iface
	 */
	extern coms_t* coms;

	/*
	 * Update the device states
	 */
	if((state != SYS_STATE_REBOOT) ||
			(state != SYS_STATE_ERROR)){
		system_t::status.device_data.alive  = true;
	}else{
		system_t::status.device_data.alive  = false;
	}

	system_t::status.device_data.alive = state;
	system_t::status.device_data.runtime = millis();

	/*
	 * Update the iface status
	 */
	system_t::status.coms_data.ip_data.status = \
			coms->wifi_if->get_status();
	system_t::status.coms_data.mqtt_data.status = \
			coms->mqtt_if->get_status();

	return true;
}

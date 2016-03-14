/*
 * system.h
 *
 *  Created on: Jul 4, 2015
 *      Author: francis-ccs
 */

#ifndef SERVICES_SYSTEM_SYSTEM_H_
#define SERVICES_SYSTEM_SYSTEM_H_

#include <configs.h>
#include <status_codes.h>
#include <task/scheduler.h>
#include <platform/others/queue.h>
#include <platform/others/caches.h>
#include <service/services/coms/coms.h>
#include <platform/sensor/sensor/sensor.h>

#include <driverlib/prcm.h>

namespace system_base {

	/**< Data typedef */
	typedef void* data_t;

	/**
	 * @brief The cache list to update
	 */
	typedef void* cache_node_t;

	/**
	 * @brief Callback to update the cache
	 */
	typedef bool (*cache_callback_t)();
	typedef bool (*sensor_cache_cb_t)(sensor*);

	typedef union {
		cache_callback_t 	f_ptr;
		sensor_cache_cb_t 	m_ptr;
	}cb;

	/**
	 * @brief The cache types
	 */
	typedef struct cache_list_t{

		cache_t				type;
		msg_type_t			msg;
		cache_node_t 		node;
		cb					fxn;
		cache_list_t* 		next;
		cache_list_t* 		prev;
		sensor_t* 			sensor;
	}cache_list_t;

	/**
	 * @brief The system BIOS interface
	 *
	 * This class contains the interface to the task sceduler and
	 * execution medium for the system.
	 */

	/*
	 * Caches
	 */
	static status_cache_t 		status;
	static heartbeat_cache_t 	heart;

	static cache_list_t 		heartbeat_cache;
	static cache_list_t 		status_cache;

	/*
	 * Sensors
	 */
	static sensor_t* sensors_base[NUMBER_OF_SENSORS + 1];

	/*
	 * The cache register
	 */
	static cache_list_t* list;

	/*
	 * Sequencer
	 */
	static scheduler_t* scheduler;

	/*
	 * State
	 */
	static sys_state_t state;

	/**
	 * Coms class
	 */
	static coms_service* system_coms;

	/**
	 * Prototypes for references
	 *
	 *  -- PUBLIC
	 */

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
	static status_code_t BIOS_setup(sensor_t* sensors[]);

	/**
	 * @brief Reboots a component
	 *
	 * @param type			The component to reboot
	 */
	static void BIOS_reboot(bios_reboot_t type);

	/**
	 * @brief Alerts the system
	 *
	 * @param alert			The alert type
	 */
	static status_code_t BIOS_alert(bios_alerts_t alert);

	/**
	 * @brief Registers a sensor cache and maps it to the system
	 *
	 * @param sensor		The sensor to map the cache from
	 */
	static status_code_t BIOS_register(sensor_t* sensor);

	/**
	 * @brief Update the caches
	 *
	 * @param type			The cache to update, by default set to all
	 */
	static status_code_t BIOS_update(cache_t type);

	/**
	 * @brief Boots the systems
	 *
	 * @param tasks			The tasks to boot and add to the execution pool.
	 */
	static status_code_t BIOS_boot(task_t* tasks[]);

	/**
	 * @brief Runs the BIOS
	 */
	static void BIOS_run();

	/**
	 * @brief Suspends a task
	 *
	 * @param id			The task id to suspend
	 */
	static void BIOS_suspend(thread_id_t id);

	/**
	 * @brief Gets the cache of type specified.
	 *
	 * @param type			The type of cache to get
	 * @return cache		The cache returned
	 */
	static cache_list_t* BIOS_cache(cache_t type);

	/**
	 * @brief Sets the new state of the system
	 *
	 * @param state			The state
	 */
	static status_code_t BIOS_state(sys_state_t state);

	/**
	 * @brief Hangs the OS
	 */
	static void BIOS_hang();

	/**
	 * @brief Register the coms within the system.
	 */
	static status_code_t BIOS_register_coms(coms_service* coms);

	/**
	 * @brief Connects the interfaces
	 */
	static status_code_t BIOS_connect();

	/**
	 * Implementation
	 *
	 * 	-- PRIVATE
	 */

	/**
	 * @brief Updates the heartbeat cache
	 */
	static bool update_heartbeat_cache();

	/**
	 * @brief Updates the status cache
	 */
	static bool update_status_cache();


	/**
	 * Implementation
	 */

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
	static status_code_t BIOS_setup(sensor_t** sensors){

		/*
		 * Setup the heartbeat cache
		 */
		system_base::heartbeat_cache.fxn.f_ptr 		= update_heartbeat_cache;
		system_base::heartbeat_cache.node			= &system_base::heart;
		system_base::heartbeat_cache.type 			= CACHE_TYPE_HEARTBEAT;
		system_base::heartbeat_cache.msg			= MSG_TYPE_HEARTBEAT;
		system_base::heartbeat_cache.next			= &system_base::status_cache;
		system_base::heartbeat_cache.prev			= NULL;
		system_base::heartbeat_cache.sensor			= NULL; 	// No sensor

		/*
		 * Setup the status cache
		 */
		system_base::status_cache.fxn.f_ptr 		= system_base::update_status_cache;
		system_base::status_cache.node				= &system_base::status;
		system_base::status_cache.type 				= CACHE_TYPE_STATUS;
		system_base::status_cache.msg				= MSG_TYPE_STATUS;
		system_base::status_cache.next				= NULL;
		system_base::status_cache.prev				= &system_base::heartbeat_cache;
		system_base::status_cache.sensor			= NULL; 	// No sensor

		/*
		 * Create the list
		 */
		list = &heartbeat_cache;

		/*
		 * Boot the scheduler
		 */
		system_base::scheduler = new scheduler_t();

		/*
		 * Add the sensors
		 */

		// copy over the sensor pointers.
		for(uint8_t i = 0; i < (NUMBER_OF_SENSORS +1); i ++){
			system_base::sensors_base[i] = sensors[i];
		}
		NOTIFY_INFO("System BIOS Booted.");
		digitalWrite(STATUS_LED, HIGH);
		return STATUS_OK;
	}

	/*
	 * Testing
	 */

	/**
	 * @brief Reboots a component
	 *
	 * @param type			The component to reboot
	 */
	static void BIOS_reboot(bios_reboot_t type){

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

		case BIOS_REBOOT_OS:

			/*
			 * HAng the OS and reboot manually
			 */
			system_base::BIOS_hang();
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
	static status_code_t BIOS_alert(bios_alerts_t alert){
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
		return STATUS_OK;
	}

	/*
	 * Caching
	 */

	/**
	 * @brief Registers a sensor cache and maps it to the system
	 *
	 * @param sensor		The sensor to map the cache from
	 */
	static status_code_t BIOS_register(sensor_t* sensor){

		// Containers
		cache_list_t* 	temp 			= system_base::list;
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
				system_base::BIOS_alert(BIOS_ALERT_REGISTER_FAIL);
				system_base::BIOS_reboot(BIOS_REBOOT_OS);
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

		return STATUS_OK;
	}

	/**
	 * @brief Update the caches
	 *
	 * @param type			The cache to update, by default set to all
	 */
	static status_code_t BIOS_update(cache_t type){

		// Containers
		bool 			rc;
		cache_list_t* 	temp 			= system_base::list;

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
				if((temp->type != CACHE_TYPE_HEARTBEAT) &&
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
				if((type != CACHE_TYPE_HEARTBEAT) &&
						(type != CACHE_TYPE_STATUS)){

					/*
					 * We need to update the caches that require a sensor handle
					 */
					temp = (system_base::BIOS_cache(type));
					rc = temp->fxn.m_ptr(temp->sensor);

				}else{

					/*
					 * OS caches that do not need sensor handle
					 */
					rc = (system_base::BIOS_cache(type))->fxn.f_ptr();
				}
			}

			/*
			 * Check for success
			 */
			if(!rc){

				/*
				 * Problem with the update
				 */
				system_base::BIOS_alert(BIOS_ALERT_UPDATE_FAIL);
				system_base::BIOS_reboot(BIOS_REBOOT_OS);
				break;
			}
			else{
				return STATUS_OK;
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
	static status_code_t BIOS_boot(task_t* tasks[]){
		/*
		 * Loop the task pointers and add them to our scheduler
		 */
		for(uint8_t i = 0; i < TASK_NUMBER; i ++){

			NOTIFY_INFO("Adding task: " + String(tasks[i]->_id));
			system_base::scheduler->add_task(tasks[i]);
		}
		return STATUS_OK;
	}

	/**
	 * @brief Runs the BIOS
	 */
	static void BIOS_run(){

		/*
		 * We run the scheduled tasks
		 */
		NOTIFY_INFO("Running the scheduler.");
		BIOS_state(SYS_STATE_ACTIVE);
		system_base::scheduler->run();
	}

	/**
	 * @brief Suspends a task
	 *
	 * @param id			The task id to suspend
	 */
	static void BIOS_suspend(thread_id_t id){
		/*
		 * We get the thread id
		 */
		NOTIFY_INFO("Suspending thread id: " \
				+ String(id));
		system_base::scheduler->suspend_queue->push(id);
	}

	/**
	 * @brief Hangs the OS
	 */
	static void BIOS_hang(){
		while(true){
			sleep(1);
		}
	}

	/**
	 * @brief Gets the cache of type specified.
	 *
	 * @param type			The type of cache to get
	 * @return cache		The cache returned
	 */
	static cache_list_t* BIOS_cache(cache_t type){

		// Container
		cache_list_t* 	temp 			= system_base::list;

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
	static status_code_t BIOS_state(sys_state_t state){
		/*
		 * Set the state
		 */
		system_base::state = state;
		return STATUS_OK;
	}

	/**
	 * @brief Register the coms within the system.
	 */
	static status_code_t BIOS_register_coms(coms_service* coms){
		system_base::system_coms = coms;
		return STATUS_OK;
	}

	/**
	 * @brief Connects the interfaces
	 */
	static status_code_t BIOS_connect(){
		if(system_base::system_coms->connect(INTERFACE_BOTH) != STATUS_OK){
			return STATUS_ERR_DENIED;
		}
		else{
			return STATUS_OK;
		}
	}

	/**
	 * @brief Updates the heartbeat cache
	 */
	static bool update_heartbeat_cache(){

		/*
		 * Update the heartbeat
		 */
		if((state != SYS_STATE_REBOOT) ||
				(state != SYS_STATE_ERROR)){
			system_base::heart.heartbeat_data.alive = true;
		}else{
			system_base::heart.heartbeat_data.alive = false;
		}

		system_base::heart.heartbeat_data.state = state;
		return true;
	}

	/**
	 * @brief Updates the status cache
	 */
	static bool update_status_cache(){

		/*
		 * Update the device states
		 */
		if((state != SYS_STATE_REBOOT) ||
				(state != SYS_STATE_ERROR)){
			system_base::status.device_data.alive  = true;
		}else{
			system_base::status.device_data.alive  = false;
		}

		system_base::status.device_data.alive = state;
		system_base::status.device_data.runtime = millis();
		return true;
	}
}

#endif /* SERVICES_SYSTEM_SYSTEM_H_ */

/*
 * system.h
 *
 *  Created on: Jul 4, 2015
 *      Author: francis-ccs
 */

#ifndef SERVICES_SYSTEM_SYSTEM_H_
#define SERVICES_SYSTEM_SYSTEM_H_

#include <task/task.h>
#include <status_codes.h>
#include <platform/platform.h>

/**
 * @brief Types of BIOS alarms
 */
typedef enum {

	BIOS_ALERT_REBOOT,         			//!< BIOS_ALERT_REBOOT
	BIOS_ALERT_COMS_FAIL,      			//!< BIOS_ALERT_COMS_FAIL
	BIOS_ALERT_SENSOR_FAIL,    			//!< BIOS_ALERT_SENSOR_FAIL
	BIOS_ALERT_SYSTEM_FAIL,    			//!< BIOS_ALERT_SYSTEM_FAIL
	BIOS_ALERT_PROCESSING_FAIL,			//!< BIOS_ALERT_PROCESSING_FAIL
	BIOS_ALERT_REGISTER_FAIL,			//!< BIOS_ALERT_REGISTER_FAIL
	BIOS_ALERT_UPDATE_FAIL,				//!< BIOS_ALERT_UPDATE_FAIL
	BIOS_ALERT_SCHEDULER_FAIL,			//!< BIOS_ALERT_SCHEDULER_FAIL
	BIOS_ALERT_TASK_FAIL				//!< BIOS_ALERT_TASK_FAIL
}bios_alerts_t;

/**
 * Types of reboot requests
 */
typedef enum {

	/*
	 * Services
	 *
	 * 	- System BIOS services
	 * 	- Communication services
	 */
	BIOS_REBOOT_SYS_SOFT,				//!< BIOS_REBOOT_SYS_SOFT
	BIOS_REBOOT_SYS_HARD,				//!< BIOS_REBOOT_SYS_HARD
	BIOS_REBOOT_COMS,   				//!< BIOS_REBOOT_COMS

	/*
	 * Interfaces
	 *
	 * 	- MQTT Interface
	 * 	- WIFI Interface
	 * 	- Sensor Interface
	 * 		- I2C
	 * 		- SPI
	 */
	BIOS_REBOOT_MQTT,    				//!< BIOS_REBOOT_MQTT
	BIOS_REBOOT_WIFI,    				//!< BIOS_REBOOT_WIFI

	/*
	 * OS
	 */
	BIOS_REBOOT_OS,       				//!< BIOS_REBOOT_OS
	BIOS_REBOOT_SCHEDULER,				//!< BIOS_REBOOT_SCHEDULER
	BIOS_REBOOT_TASK,					//!< BIOS_REBOOT_TASK
}bios_reboot_t;

typedef enum {

	SYS_STATE_IDLE,
	SYS_STATE_SLEEP,
	SYS_STATE_ACTIVE,
	SYS_STATE_REBOOT,
	SYS_STATE_ERROR,

}sys_state_t;

/**
 * Cache types
 */
typedef enum {

	CACHE_TYPE_HEARTBEAT,				//!< CACHE_TYPE_HEARTBEAT
	CACHE_TYPE_STATUS,   				//!< CACHE_TYPE_STATUS
	CACHE_TYPE_TEMP_DATA	= 0x0200,	//!< CACHE_TYPE_TEMP_DATA
	CACHE_TYPE_ACC_DATA  	= 0x0001,	//!< CACHE_TYPE_ACC_DATA
	CACHE_TYPE_ALL						//!< CACHE_TYPE_ALL
}cache_t;

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

/**
 * @brief The cache types
 */
typedef struct {

	cache_t				type;
	msg_type_t			msg;
	cache_node_t 		node;
	cache_callback_t	fxn;
	cache_list_t* 		next;
	cache_list_t* 		prev;
}cache_list_t;

/**
 * @brief The system BIOS interface
 *
 * This class contains the interface to the task sceduler and
 * execution medium for the system.
 */
class system_srv {

	/*
	 * Public Attributes access
	 */
	public:

		/*
		 * Caches
		 */
		static status_cache_t status;
		static heartbeat_cache_t heart;

		static cache_list_t heartbeat_cache;
		static cache_list_t status_cache;

		/*
		 * Suspending queue
		 */
		static queue_t<thread_id_t>* suspend_queue;

		/*
		 * Formatter
		 */
		static formatter_t format;

		/*
		 * Coms
		 */
		static coms_t coms;

		/*
		 * Sensors
		 */
		static sensor_t* sensors;

		/*
		 * TODO STATE, STATUS
		 */

		/*
		 * The cache register
		 */
		cache_list_t* list;

		/*
		 * Sequencer
		 */
		scheduler_t scheduler;

	/*
	 * Public method access
	 */
	public:

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
		static void BIOS_setup(sensor_t* sensors);

		/*
		 * Testing
		 */

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
		static void BIOS_alert(bios_alerts_t alert);

		/*
		 * Caching
		 */

		/**
		 * @brief Registers a sensor cache and maps it to the system
		 *
		 * @param sensor		The sensor to map the cache from
		 */
		static void BIOS_register(sensor_t* sensor);

		/**
		 * @brief Update the caches
		 *
		 * @param type			The cache to update, by default set to all
		 */
		static void BIOS_update(cache_t type);

		/*
		 * Running
		 */

		/**
		 * @brief Boots the systems
		 *
		 * @param tasks			The tasks to boot and add to the execution pool.
		 */
		static void BIOS_boot(task_t* tasks);

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
		 * @brief Hangs the OS
		 */
		static void BIOS_hang(){
			while(true);
		}

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
		static void BIOS_state(sys_state_t state);

	/*
	 * Pirvate access methods
	 */
	private:

		/*
		 * State
		 */
		sys_state_t state;

		/**
		 * @brief Updates the heartbeat cache
		 */
		static void update_heartbeat_cache();

		/**
		 * @brief Updates the status cache
		 */
		static void update_status_cache();
};

/**< Typedef */
typedef system_srv system_t;

#endif /* SERVICES_SYSTEM_SYSTEM_H_ */

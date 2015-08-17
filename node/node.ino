
/**
 * Global Library Includes
 */

/*
 * Platform includes
 */
#include <platform/platform.h>

/*
 * System services includes
 */
#include <service/services.h>

/*
 * System tasks includes
 */
#include <task/tasks.h>

/*
 * Bus
 */
bus_i2c_t* bus 		= new bus_i2c_t();

/*
 * Sensors
 */
tmp006_t* tmp006 	= new tmp006_t(bus);
bma222_t* bma222 	= new bma222_t(bus);

/*
 * Sensor list
 */
sensor_t sensors[]	= {
		tmp006,
		bma222,
		NULL
};

/*
 * Task list
 */
static task_t* tasks 	= {

#ifdef DAQ_TASK_ENABLE
		/*
		 * We first get the information from the sensors and
		 * store it in the local sensors cache where the data
		 * resided until the update task gets to them.
		 *
		 * Thread Id 	= 1
		 * Interval 	= 100ms
		 */
		new daq_t		(system_t::sensors /*	*/),
#endif

#ifdef UPDATE_TASK_ENABLE
		/*
		 * Once the data has been read and stroed within the sensor
		 * local cache, the update task copies the data to BIOS mapped
		 * structures that are accessible by the OS.
		 *
		 * Thread Id 	= 2
		 * Interval 	= 500ms
		 */
		new update_t	(/*  NULL         		*/),
#endif

#ifdef PUBLISH_TASK_ENABLE
		/*
		 * Once this data has been updated, we then formulate a publish
		 * request with the data on hand and then publish the data to the
		 * MQTT broker.
		 *
		 * Thread Id 	= 3
		 * Interval 	= 500ms + 1sec sleep
		 */
		new publish_t	(system_t::coms /*		*/),
#endif

#ifdef IDLE_TASK_ENABLE
		/*
		 * The system after that goes into an idle state to make sure the
		 * requests are correctly processed by the server. This was
		 * tasks is implemented to allow for the biggest bottleneck of the
		 * system to be able to process the messages acuretly without overflow.
		 *
		 * Thread Id 	= 0
		 * Interval 	= 500ms
		 */
		new idle_t		(system_t::coms /*		*/),
#endif
};

void setup() {

	/*
	 * In this function context, we would setup the systems peripherals
	 * and instantiate a device database, containing the received data
	 * and the device instances.
	 *
	 * Boot:
	 * 	- tmp006,
	 * 	- bma222,
	 * 	- wifi,
	 * 	- mqtt,
	 * 	- db
	 */

	/*
	 * Init the system
	 */
	system_t::BIOS_setup(sensors);

	/*
	 * Register the sensor caches
	 */
	system_t::BIOS_register(tmp006);
	system_t::BIOS_register(bma222);

	/*
	 * Boot the OS
	 */
	system_t::BIOS_boot(tasks);
}

void loop() {

	/*
	 * This is the main loop of the system.
	 * This function will call our BIOS loop and thus
	 * will never return. In the BIOS loop, our scheduler will
	 * queue up the task pointers to execute one task at a time.
	 *
	 * Tasks:
	 * 	- DAQ
	 * 	- PUBLISH
	 * 	- UPDATE
	 * 	- IDLE
	 */

	/*
	 * Run the OS
	 */
	system_t::BIOS_run();
}

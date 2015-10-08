/*
 * daq.h
 *
 *  Created on: Jul 4, 2015
 *      Author: francis-ccs
 */

#ifndef SERVICES_DATA_DAQ_H_
#define SERVICES_DATA_DAQ_H_

/*
 * Necessary
 */
#include <configs.h>
#include <task/task.h>

/*
 * Peripherals
 */
#include <platform/platform.h>
#include <platform/sensor/sensor/sensor.h>
#include <service/services/system/system.h>

/*
 * Local task defines
 */
#define DAQ_TASK_INTERVAL			(100)	// EXECUTE EVERY 100 ms
#define DAQ_TASK_ITERATIONS			(-1)	// NO LIMIT
#define DAQ_THREAD_ID				(1)		// DEFAULT ID

/**
 * @brief This is the daq task interface.
 *
 * We use this class as a static task allocation.
 * The daq task is the task that gets all the sensor
 * data from the onboard sensors.
 */
class daq : public task_t {

	/*
	 * Public access methods
	 */
	public:

		/**
		 * @brief The default constructor
		 *
		 * This is the daq task constructor. It sets all
		 * internal parameters to the task itself and enables it.
		 *
		 * @param sensors		The sensor interfaces
		 */
		daq(sensor_t* sensors[]);

		/**
		 * @brief The default deconstructor
		 */
		~daq(){};

	/*
	 * Private access methods
	 */
	private:

		/*
		 * Private attributes
		 */
		static const uint32_t 	interval		= DAQ_TASK_INTERVAL;
		static const uint32_t 	iterations		= DAQ_TASK_ITERATIONS;

		/*
		 * Service handle
		 */
		static sensor_t**		sensors;

		/**
		 * @brief Daq task callback
		 *
		 * This is the daq task callback method that will
		 * get hooked into the squeduler framework.
		 */
		static void daq_task_cb();
};

/**< Typedef */
typedef daq daq_t;

#endif /* SERVICES_DATA_DAQ_H_ */

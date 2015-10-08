/*
 * update.h
 *
 *  Created on: Jul 29, 2015
 *      Author: fpapinea
 */

#ifndef SERVICES_TASKS_UPDATE_H_
#define SERVICES_TASKS_UPDATE_H_

/*
 * Necessary
 */
#include <configs.h>
#include <task/task.h>

/*
 * Peripherals
 */
#include <service/services/system/system.h>

/*
 * Local task defines
 */
#define UPDATE_TASK_INTERVAL			(500)	// EXECUTE EVERY 500 ms
#define UPDATE_TASK_ITERATIONS			(-1)	// NO LIMIT
#define UPDATE_THREAD_ID				(2)		// DEFAULT ID

/**
 * @brief This is the cache updates task interface.
 *
 * We use this class as a static task allocation.
 * The update task is the task that updates the local and
 * global mapped data caches.
 */
class update : public task_t {

	/*
	 * Public access methods
	 */
	public:

		/**
		 * @brief The default constructor
		 *
		 * This is the update task constructor. It sets all
		 * internal parameters to the task itself and enables it.
		 */
		update();

		/**
		 * @brief The default deconstructor
		 */
		~update(){};

	/*
	 * Private access methods
	 */
	private:

		/*
		 * Private attributes
		 */
		static const uint32_t 	interval		= UPDATE_TASK_INTERVAL;
		static const uint32_t 	iterations		= UPDATE_TASK_ITERATIONS;

		/**
		 * @brief Update task callback
		 *
		 * This is the update task callback method that will
		 * get hooked into the squeduler framework.
		 */
		static void update_task_cb();
};

/**< Typedef */
typedef update update_t;

#endif /* SERVICES_TASKS_UPDATE_H_ */

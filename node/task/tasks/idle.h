/*
 * idle.h
 *
 *  Created on: Jul 29, 2015
 *      Author: fpapinea
 */

#ifndef SERVICES_TASKS_IDLE_H_
#define SERVICES_TASKS_IDLE_H_

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
#define IDLE_TASK_INTERVAL			(500)	// EXECUTE EVERY 500 ms
#define IDLE_TASK_ITERATIONS		(-1)	// NO LIMIT
#define IDLE_THREAD_ID				(0)		// DEFAULT ID

using namespace system_base;

/**
 * @brief This is the idle task interface.
 *
 * We use this class as a static task allocation.
 * The idle task is a task that is present to buffer the
 * cpu and to make it sleep when needed.
 */
class idle : public task_t {

	/*
	 * Public access methods
	 */
	public:

		/**
		 * @brief The default constructor
		 *
		 * This is the idle task constructor. It sets all
		 * internal parameters to the task itself and enables it.
		 */
		idle();

		/**
		 * @brief The default deconstructor
		 */
		~idle(){};

	/*
	 * Private access methods
	 */
	private:

		/*
		 * Private attributes
		 */
		static const uint32_t 	interval		= IDLE_TASK_INTERVAL;
		static const uint32_t 	iterations		= IDLE_TASK_ITERATIONS;

		/**
		 * @brief Idle task callback
		 *
		 * This is the idle task callback method that will
		 * get hooked into the squeduler framework.
		 */
		static void idle_task_cb();
};

/**< Typedef */
typedef idle idle_t;

#endif /* SERVICES_TASKS_IDLE_H_ */

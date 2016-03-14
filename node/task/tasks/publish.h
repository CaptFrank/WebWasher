/*
 * publish.h
 *
 *  Created on: Jul 29, 2015
 *      Author: fpapinea
 */

#ifndef SERVICES_TASKS_PUBLISH_H_
#define SERVICES_TASKS_PUBLISH_H_

/*
 * Necessary
 */
#include <configs.h>
#include <task/task.h>

/*
 * Peripherals
 */
#include <service/services/system/system.h>
#include <service/services/formatter/formatter.h>


/*
 * Local task defines
 */
#define PUB_TASK_INTERVAL			(500)	// EXECUTE EVERY 500 ms
#define PUB_TASK_ITERATIONS			(-1)	// NO LIMIT
#define PUB_THREAD_ID				(3)		// DEFAULT ID

#define PUB_SLEEP					(1000)	// Sleep for 1sec

using namespace system_base;

/**
 * @brief This is the publish task interface.
 *
 * We use this class as a static task allocation.
 * The publish task is a task that is reponsible in taking
 * the internal caches, formatting them into JSON reponses and
 * publishing them to the mqtt broker.
 */
class publish : public task_t {

	/*
	 * Public access methods
	 */
	public:

		/**
		 * @brief The default constructor
		 *
		 * This is the publish task constructor. It sets all
		 * internal parameters to the task itself and enables it.
		 */
		publish();

		/**
		 * @brief The default deconstructor
		 */
		~publish(){};

	/*
	 * Private access methods
	 */
	private:

		/*
		 * Private attributes
		 */
		static const uint32_t 	interval		= PUB_TASK_INTERVAL;
		static const uint32_t 	iterations		= PUB_TASK_ITERATIONS;

		/**
		 * @brief Publish task callback
		 *
		 * This is the publish task callback method that will
		 * get hooked into the squeduler framework.
		 */
		static void publish_task_cb();
};

/**< Typedef */
typedef publish publish_t;

#endif /* SERVICES_TASKS_PUBLISH_H_ */

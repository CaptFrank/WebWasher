/*
 * scheduler.h
 *
 *  Created on: Jul 29, 2015
 *      Author: fpapinea
 */

#ifndef TASK_SCHEDULER_H_
#define TASK_SCHEDULER_H_

/*
 * Include the task definition
 */
#include <task/task.h>
#include <status_codes.h>
#include <platform/others/queue.h>

/**
 * @brief The scheduler interface
 *
 * This is the scheduler interface class. We use it as a
 * means to schdeuling new tasks that will run concurrently
 * on the system.
 */
class scheduler {

	/*
	 * Private access methods
	 */
	private:

		/*
		 * Friend class
		 */
		friend task;

		/*
		 * Tasks pointers
		 */
		task_t* 	first;
		task_t* 	last;

	/*
	 * Public access methods
	 */
	public:

		/*
		 * Suspending queue
		 */
		queue<thread_id_t>* suspend_queue;

		/**
		 * @brief The default object constructor.
		 */
		scheduler();

		/**
		 * @brief Adds a task to the task queue.
		 *
		 * @param 	task			The task pointer to add
		 * @return	status			The status to return
		 */
		status_code_t add_task(task_t* task);

		/**
		 * @brief Deletes a task from the task queue.
		 *
		 * @param 	task			The task to delete
		 * @return 	status			The status to return
		 */
		status_code_t delete_task(task_t* task);

		/**
		 * @brief Deletes all the queued tasks in the task queue.
		 */
		void disable_all();

		/**
		 * @brief Enables all the tasks in the task queue.
		 *
		 * @return	status 			The status to return
		 */
		status_code_t enable_all();

		/**
		 * @brief Runs the sceduler
		 */
		void run();

		/**
		 * @brief Disable the tasks based on the thread id in the queues
		 */
		void disable_tasks();

		/**
		 * @brief The default deconstructor.
		 */
		~scheduler(){}

		/**
		 * @brief Gets the queue
		 */
		queue<thread_id_t>* get_queue(){
			return this->suspend_queue;
		}
};

/**< @brief Typedef for the scheduler */
typedef scheduler scheduler_t;

#endif /* TASK_SCHEDULER_H_ */

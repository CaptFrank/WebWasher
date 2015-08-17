/*
 * scheduler.cpp
 *
 *  Created on: Jul 29, 2015
 *      Author: fpapinea
 */

#include <task/scheduler.h>

/**
 * @brief The default object constructor.
 */
scheduler::scheduler(){

	/*
	 * Set all attributes to null
	 */
	first 	= NULL;
	last 	= NULL;
}

/**
 * @brief Adds a task to the task queue.
 *
 * @param 	task			The task pointer to add
 * @return	status			The status to return
 */
status_code_t scheduler::add_task(task_t* task){

	/*
	 * Add a task to the scheduler
	 */
	task->scheduler = this;

	/*
	 * If there is no task registered
	 */
	if(first == NULL){
		first = task;
		task->prev = NULL;
	}else{

		/*
		 * New task
		 */
		task->prev = last;
		last->next = task;
	}

	/*
	 * Update
	 */
	task->next = NULL;
	last = task;

	return STATUS_OK;
}

/**
 * @brief Deletes a task from the task queue.
 *
 * @param 	task			The task to delete
 * @return 	status			The status to return
 */
status_code_t scheduler::delete_task(task_t* task){

	/*
	 * delete the task
	 */

	if (task->prev == NULL) {
		if (task->next == NULL) {

			/*
			 * No more tasks
			 */
			first = NULL;
			last = NULL;
			return STATUS_OK;
		}else {

			/*
			 * Delete a leaf
			 */
			task->next->prev = NULL;
			first = task->next;
			task->next = NULL;
			return STATUS_OK;
		}
	}

	/*
	 * Only one task
	 */
	if (task->next == NULL) {

		task->prev->next = NULL;
		last = task->prev;
		task->prev = NULL;
		return STATUS_OK;
	}


	/*
	 * Update
	 */
	task->prev->next = task->next;
	task->next->prev = task->prev;
	task->prev = NULL;
	task->next = NULL;
	return STATUS_OK;
}

/**
 * @brief Deletes all the queued tasks in the task queue.
 */
void scheduler::disable_all(){

	/*
	 * Temp
	 */
	task_t* temp = first;
	while(temp){
		temp->disable();
		temp = temp->next;
	}
}

/**
 * @brief Enables all the tasks in the task queue.
 *
 * @return	status 			The status to return
 */
status_code_t scheduler::enable_all(){

	/*
	 * Temp
	 */
	task_t* temp = first;
	while(temp){
		temp->enable();
		temp = temp->next;
	}
	return STATUS_OK;
}

/**
 * @brief Disable the tasks based on the thread id in the queues
 */
void scheduler::disable_tasks(){

	/*
	 * Temp
	 */
	task_t *current = first;
	uint8_t count	= system_t::suspend_queue->count();

	if (count > 0){
		/*
		 * disable the tasks
		 */
		for(uint8_t i = 0; i < count, i++){

			/*
			 * Check the disable queue for disabling thread
			 */
			thread_id_t id = \
					(thread_id_t)system_t::suspend_queue->pop();

			// Disable the task
			while(current){

				/*
				 * disable the task
				 */
				if(current->_id == id){
					current->_enabled = false;
				}

				/*
				 * Update to the next task
				 */
				current = current->next;
			}
		}
	}
}


/**
 * @brief Runs the sceduler
 */
void scheduler::run(){

	/*
	 * Check to see if we need to disable any tasks
	 */
	disable_tasks();

	/*
	 * Update the state
	 */
	system_t::BIOS_state(SYS_STATE_ACTIVE);

	/*
	 * Temp
	 */
	task_t *current = first;

	/*
	 * Until all tasks are ran
	 */
	while (current) {

		do {

			/*
			 * If the task is not enabled
			 */
			if (!current->_enabled) {
				break;
			}

			/*
			 * IF the iterations are now elapsed
			 */
	   		if (current->_iterations == 0) {

	   			/*
	   			 * Do we disable it?
	   			 */
				if (current->_disable_on_last_iteration){

					/*
					 * Disable
					 */
					current->disable();
				}
				break;
			}

	   		/*
	   		 * If the interval is still valid
	   		 */
			if (current->_interval > 0) {

				/*
				 * Set the target time to execute
				 */
				unsigned long target_millis = \
						current->_previous_millis + current->_interval;

				/*
				 * Still too small?
				 */
				if (target_millis <= millis()) {

					/*
					 * Update the time
					 */
					if (current->_iterations > 0){

						current->iIterations--;  // do not decrement (-1) being a signal of eternal task
					}

					/*
					 * Update
					 */
					current->_previous_millis += current->_interval;
					current->_overrun = (long) (current->_previous_millis + current->_interval - millis());

					/*
					 * Execute the task
					 */
					if (current->_callback) {
						(*(current->_callback))();
					}
					break;
				}
			}else {

				/*
				 * Update
				 */
				if (current->_iterations > 0){
					current->_iterations--;  // do not decrement (-1) being a signal of eternal task
				}

				/*
				 * Execute
				 */
				if (current->_callback) {
					(*(current->_callback))();
				}
			}

		} while (false); //guaranteed single run - allows use of "break" to exit

		/*
		 * Update to the next task
		 */
		current = current->next;
	}
}

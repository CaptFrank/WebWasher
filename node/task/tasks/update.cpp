/*
 * update.cpp
 *
 *  Created on: Jul 29, 2015
 *      Author: fpapinea
 */

#include <task/tasks/update.h>

/**
 * @brief The default constructor
 *
 * This is the update task constructor. It sets all
 * internal parameters to the task itself and enables it.
 */
update::update(): \
	task_t(interval, iterations, update_task_cb, UPDATE_THREAD_ID){

	/*
	 * Enable the task from the get go
	 */
	enable();
}

/**
 * @brief Update task callback
 *
 * This is the update task callback method that will
 * get hooked into the squeduler framework.
 */
void update::update_task_cb(){

	/*
	 * We call the BIOS to update the tasks
	 * iteratively
	 */
	system_t::BIOS_update(CACHE_TYPE_ALL);
	return;
}

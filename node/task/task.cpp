/*
 * task.cpp
 *
 *  Created on: Jul 29, 2015
 *      Author: fpapinea
 */

#include <task/task.h>

/**
 * @brief Default constructor for the task class.
 *
 * @param interval		- the interval
 * @param iterations	- the iterations
 * @param callback		- the callback function
 * @param id			- the thread id
 */
task::task(uint32_t interval, uint32_t iterations, callback_t callback, thread_id_t id){

	/*
	 * Reset the device
	 */
	reset();

	/*
	 * Explicit set
	 */
	set(interval, iterations, callback);

	/*
	 * Set the thread id
	 */
	_id = id;
}

/*
 * Control
 */

/**
 * @brief Enables the task and schedules
 */
void task::enable(){

	/*
	 * Enable the task
	 */
	_enabled = true;
	_previous_millis = millis() - _interval;
}

/**
 * @brief Delays Task for execution after a delay
 *
 * Leaves task enabled or disabled if aDelay is zero, delays for the original
 * scheduling interval from now
 *
 * @param delay			- delay before starting the task
 */
void task::task_delay(uint32_t delay){

	/*
	 * Set a delay
	 */
	if(! delay){
		delay = _interval;
	}
	_previous_millis = millis() - _interval  + delay;
}

/**
 * @brief Enables the task and schedules it for execution after a delay
 *
 * @param delay			- delay before starting the task
 */
void task::enable_delayed(uint32_t delay){

	/*
	 * Set enabled
	 */
	_enabled = true;
	task_delay(delay);
}

/**
 * @brief Restarts task
 *
 * Task will run number of iterations again
 */
void task::restart(){

	/*
	 * Reset the iteration count
	 */
	_iterations = _set_iterations;
	enable();
}

/**
 * @brief Restarts task delayed
 *
 * Task will run number of iterations again
 *
 * @param delay			- delay the restart of the task
 */
void task::restart_delayed(uint32_t delay){

	/*
	 * Delay the restart
	 */
	_iterations = _set_iterations;
	enable_delayed(delay);
}

/**
 * @brief Disables task
 *
 * Task will no loner be executed by the scheduler
 */
void task::disable(){

	/*
	 * Disable the task
	 */
	_enabled = false;
}

/**
 * @brief Explicitly get Task execution parameters
 *
 * @param interval			- execution interval in ms
 * @param iterations		- number of iterations, use -1 for no limet
 * @param callback			- pointer to the callback function which executes the task actions
 */
void task::get(uint32_t* interval, uint32_t* iterations, callback_t* callback){

	/*
	 * Return the data
	 */
	*interval 	= _interval;
	*iterations = _iterations;
	*callback 	= _callback;
}

/*
 * Setters
 */

/**
 * @brief Explicitly set Task execution parameters
 *
 * @param interval			- execution interval in ms
 * @param iterations		- number of iterations, use -1 for no limet
 * @param callback			- pointer to the callback function which executes the task actions
 */
void task::set(uint32_t interval, uint32_t iterations, callback_t callback){

	/*
	 * Set internals
	 */
	_interval 	= interval;
	_iterations = iterations;
	_callback 	= callback;
}

/**
 * @brief Sets the execution interval.
 *
 * Task execution is delayed for aInterval
 * Use  enable() to schedule execution ASAP
 *
 * @param interval 			- new execution interval
 */
void task::set_interval(uint32_t interval){

	/*
	 * Set the interval
	 */
	_interval = interval;
	delay(DEFAULT_DELAY);
}


/**
 * @brief Resets (initializes) the task
 *
 * Task is not enabled and is taken out
 * out of the execution chain as a result
 */
void task::reset(){

	/*
	 * Reset the object
	 */
	_enabled 						= false;
	_previous_millis 				= 0;
	prev 							= NULL;
	next 							= NULL;
	scheduler 						= NULL;
	_disable_on_last_iteration 		= false;
	_overrun 						= 0;
}

/*
 * task.h
 *
 *  Created on: Jul 29, 2015
 *      Author: fpapinea
 */

#ifndef TASK_TASK_H_
#define TASK_TASK_H_

#include "Energia.h"
#include <configs.h>

/**
 * @brief callback definition
 */
typedef void (*callback_t)();

/**
 * @brief Thread id
 */
typedef uint8_t thread_id_t;

/**
 * @brief This is the task interface
 *
 * We use this class as an interface to the task creation
 * and management. This class is the base class for all interfaced
 * tasks.
 */
class task {

	/*
	 * Public access attributes
	 */
	public:

		/*
		 * Task list pointers
		 */
		task* 					prev;
		task* 					next;

		/*
		 * Scheduler
		 */
		void* 					scheduler;

		/*
		 * Private attributes
		 */
		volatile bool 			_enabled;
		volatile bool 			_disable_on_last_iteration;

		volatile uint32_t 		_interval;
		volatile uint32_t 		_iterations;

		volatile uint32_t 		_previous_millis;
		volatile uint32_t 		_overrun;

		uint32_t 				_set_iterations;

		/*
		 * The task callback
		 */
		callback_t 				_callback;

		/*
		 * Thread id to identify the thread
		 */
		thread_id_t				_id;

	/*
	 * Public access methods
	 */
	public:

		/*
		 * Construction
		 */

		/**
		 * @brief Default constructor for the task class.
		 *
		 * @param interval		- the interval
		 * @param iterations	- the iterations
		 * @param callback		- the callback function
		 * @param id			- the thread id
		 */
		task(uint32_t interval, uint32_t iterations, callback_t callback, thread_id_t id);

		/**
		 * @brief The default deconstructor for the class object.
		 */
		~task(){}

		/*
		 * Control
		 */

		/**
		 * @brief Enables the task and schedules
		 */
		void enable();

		/**
		 * @brief Delays Task for execution after a delay
		 *
		 * Leaves task enabled or disabled if aDelay is zero, delays for the original
		 * scheduling interval from now
		 *
		 * @param delay			- delay before starting the task
		 */
		void task_delay(uint32_t delay);

		/**
		 * @brief Enables the task and schedules it for execution after a delay
		 *
		 * @param delay			- delay before starting the task
		 */
		void enable_delayed(uint32_t delay);

		/**
		 * @brief Restarts task
		 *
		 * Task will run number of iterations again
		 */
		void restart();

		/**
		 * @brief Restarts task delayed
		 *
		 * Task will run number of iterations again
		 *
		 * @param delay			- delay the restart of the task
		 */
		void restart_delayed(uint32_t delay);

		/**
		 * @brief Disables task
		 *
		 * Task will no loner be executed by the scheduler
		 */
		void disable();

		/**
		 * @brief Disables the task after the last iteration
		 *
		 * @param disable		- disable bool
		 */
		void disable_on_last_iteration(bool disable){
			_disable_on_last_iteration = disable;
		}

		/*
		 * Getters
		 */

		/**
		 * @brief Returns true if the task is enabled.
		 *
		 * @return bool 		- true if the task is enabled
		 */
		bool is_enabled(){
			return _enabled;
		}

		/**
		 * @brief Returns the first iteration boolean.
		 *
		 * @return bool				- true if its the first iteration
		 */
		bool is_first_interation(){
			return (_iterations >= (_set_iterations - 1));
		}

		/**
		 * @brief Return the last iteration boolean.
		 *
		 * @return bool				- true if its the last iteration
		 */
		inline bool is_last_iteration(){
			return (_iterations == 0);
		}

		/**
		 * @brief Gets the interval
		 *
		 * @return interval			- the interval
		 */
		uint32_t get_interval(){
			return _interval;
		}

		/**
		 * @brief Gets the iteration
		 *
		 * @return iteration		- the iteration
		 */
		uint32_t get_iterations(){
			return _iterations;
		}

		/**
		 * @brief Explicitly get Task execution parameters
		 *
		 * @param interval			- execution interval in ms
		 * @param iterations		- number of iterations, use -1 for no limet
		 * @param callback			- pointer to the callback function which executes the task actions
		 */
		void get(uint32_t* interval, uint32_t* iterations, callback_t* callback);

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
		void set(uint32_t interval, uint32_t iterations, callback_t callback);

		/**
		 * @brief Sets the execution interval.
		 *
		 * Task execution is delayed for aInterval
		 * Use  enable() to schedule execution ASAP
		 *
		 * @param interval 			- new execution interval
		 */
		void set_interval(uint32_t interval);

		/**
		 * @brief Sets the iterations
		 *
		 * @param iterations		- number of iterations to run
		 */
		void set_iterations(uint32_t iterations){
			_iterations = iterations;
		}

		/**
		 * @brief Sets a callback
		 *
		 * @param					- the new callback function
		 */
		void set_callback(callback_t fp){
			_callback = fp;
		}

	/*
	 * Private access methods
	 */
	private:

		/**
		 * @brief Resets (initializes) the task
		 *
		 * Task is not enabled and is taken out
		 * out of the execution chain as a result
		 */
		void reset();
};

/**< @brief Typedef for the task */
typedef task task_t;

#endif /* TASK_TASK_H_ */

/*
 * daq.cpp
 *
 *  Created on: Jul 4, 2015
 *      Author: francis-ccs
 */

#include <task/tasks/daq.h>

/**
 * @brief The default constructor
 *
 * This is the daq task constructor. It sets all
 * internal parameters to the task itself and enables it.
 */
daq::daq() : \
	task_t(interval, iterations, daq_task_cb, DAQ_THREAD_ID){

	/*
	 * We enable the task right away
	 */
	enable();
}

/**
 * @brief Daq task callback
 *
 * This is the daq task callback method that will
 * get hooked into the squeduler framework.
 */
void daq::daq_task_cb(){

	/*
	 * Containers
	 */
	uint8_t	  	index	= 0;
	sensor_t* 	sensor 	= NULL;

	/*
	 * In this task, we read the data from each sensor
	 * which is then stored within each sensor local cache.
	 *
	 * The update task is then invoked to update the global cache
	 * with the most pertinent data.
	 */
	do{
		sensor = (sensor_t*)&system_base::sensors_base[index];
		if(sensor){ // Valid check of the sensor
			if(sensor->run()){
				index ++;
			}else{
				system_base::BIOS_alert(BIOS_ALERT_TASK_FAIL);
				system_base::BIOS_reboot(BIOS_REBOOT_OS);
			}
		}
	}while(sensor);
}

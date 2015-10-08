/*
 * idle.cpp
 *
 *  Created on: Jul 29, 2015
 *      Author: fpapinea
 */

#include <task/tasks/idle.h>

/**
 * @brief The default constructor
 *
 * This is the idle task constructor. It sets all
 * internal parameters to the task itself and enables it.
 *
 * @param coms_srv				The coms service
 */
idle::idle() : \
		task_t(interval, iterations, idle_task_cb, IDLE_THREAD_ID){

	/*
	 * Setup the hearbeat led
	 */
	pinMode(HEARTBEAT_LED, OUTPUT);

	/*
	 * We enable the task right away
	 */
	enable();
}

/**
 * @brief Idle task callback
 *
 * This is the idle task callback method that will
 * get hooked into the squeduler framework.
 */
void idle::idle_task_cb(){

	// Iface
	extern coms_t* coms;

	/*
	 * Set the new state of the system
	 */
	system_t::BIOS_state(SYS_STATE_IDLE);

	/*
	 * Here all we do is light up the heartbeat LED and
	 * send out a heartbeat to the server.
	 */

	// Light up the led
	digitalWrite(HEARTBEAT_LED, HIGH);

	// Send the heartbeat message
	if(coms->send(MSG_TYPE_HEARTBEAT, NULL) != MQTT_SUCCESS_STATUS){

		/*
		 * We couldn't send the heartbeat message, so we reboot
		 * the problematic component.
		 *
		 * 	- In this case we would reboot the coms module.
		 */
		system_t::BIOS_alert(BIOS_ALERT_COMS_FAIL);
		system_t::BIOS_reboot(BIOS_REBOOT_COMS);
	}

	// Shutdown the led
	digitalWrite(HEARTBEAT_LED, LOW);

	/*
	 * Set the new state of the system
	 */
	system_t::BIOS_state(SYS_STATE_ACTIVE);
}

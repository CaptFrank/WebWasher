/*
 * publish.cpp
 *
 *  Created on: Jul 29, 2015
 *      Author: fpapinea
 */

#include <services/tasks/publish.h>

/**
 * @brief The default constructor
 *
 * This is the publish task constructor. It sets all
 * internal parameters to the task itself and enables it.
 *
 * @param coms_srv				The coms service
 */
publish::publish(coms_t* coms_srv) : \
	task_t(interval, iterations, publish_task_cb, PUB_THREAD_ID){

	/*
	 * Set the internal reference to the coms
	 */
	coms_service = coms_srv;

	/*
	 * Enable the task
	 */
	enable();
}

/**
 * @brief Publish task callback
 *
 * This is the publish task callback method that will
 * get hooked into the squeduler framework.
 */
void publish::publish_task_cb(){

	/*
	 * The string pointer to send
	 */
	msg_t* json;

	/*
	 * Message types to send off.
	 *
	 * - Status
	 * - Data
	 */
	cache_list_t* cache = &system_t::list;

	/*
	 * We iterate through the message types to send and we
	 * format the appropriate cache to a string to then send it off.
	 */
	do{

		/*
		 * Only publish the data and the status
		 */
		if((MSG_TYPE_HEARTBEAT != cache->msg) && \
				(MSG_TYPE_OTHER != cache->msg)){

			/*
			 * Format the cache
			 */
			json = system_t::format.format(cache->type);

			/*
			 * Send the string the the mqtt component
			 */
			system_t::coms.send(cache.msg, json);
			sleep(PUB_SLEEP); // sleep for a bit
		}
	}while(cache);
}
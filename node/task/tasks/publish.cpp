/*
 * publish.cpp
 *
 *  Created on: Jul 29, 2015
 *      Author: fpapinea
 */

#include <task/tasks/publish.h>

/**
 * @brief The default constructor
 *
 * This is the publish task constructor. It sets all
 * internal parameters to the task itself and enables it.
 */
publish::publish() : \
	task_t(interval, iterations, publish_task_cb, PUB_THREAD_ID){

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
	cache_list_t* cache = system_base::list;

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
			json = formatter_t::format(cache->type);

			/*
			 * Send the string the the mqtt component
			 */
			system_base::system_coms->send(cache->msg, json);
			delay(PUB_SLEEP); // sleep for a bit
		}

		/*
		 * Update to the next cache
		 */
		cache = cache->next;
	}while(cache);
}

/*
 * mqtt.cpp
 *
 *  Created on: Jul 4, 2015
 *      Author: francis-ccs
 */

#include "mqtt.h"
#include <platform/others/queue.h>

/**
 * Define a queue for the callback to insert the message in.
 * Stores the pointers of the structures on the heap.
 */
static queue<mqtt_message_t*>* mqtt_msg_qeue = new queue<mqtt_message_t*>();

/**
 * @breif The Default callback for the MQTT engine.
 *
 * @param topic		The topic subscribed to
 * @param payload	The payload to read
 * @param length	The length
 */
void __default_callback(char* topic, byte* payload, unsigned int length){

	// Structure
	mqtt_message_t* msg;

	/*
	 * We need to compare the topic with the registered callbacks
	 * We follow the following sequence to get the appropriate callback.
	 * 	- Tokenize the topic (i.e. split "/")
	 * 	- Compare the real topic name (i.e. /data/cmd/update)
	 * 	- Trigger the callback
	 */

	/*
	 * Notify the command
	 */
	NOTIFY_INFO("Found cmd: " + String(topic));

	/*
	 * Put the payload in a message struct and then put the pointer in
	 * the queue.
	 */
	msg = (mqtt_message_t*)malloc(sizeof(mqtt_message_t));
	msg->length 	= length;
	msg->payload 	= payload;
	msg->topic 		= topic;

	/*
	 * Push the data on the queue
	 */
	mqtt_msg_qeue->push(msg);

	/*
	 * Notify the USER
	 */
	digitalWrite(RECEIVE_LED, HIGH);
	delay(MQTT_DELAY);
	digitalWrite(RECEIVE_LED, LOW);

	/*
	 * Prevent memory leaks
	 */
	free(msg);
	return;
}

/**
 * @brief The MQTT Interface Constructor
 *
 * This is the entry method for the MQTT interface object.
 * We must pass the stack instance.
 *
 * @param wifistack		The stack instance
 * @param mqttparams	The mqtt parameters
 */
mqtt::mqtt(Client* wifistack, mqtt_params_t* mqttparams){

	/*
	 * All is good
	 */
	status = STATUS_OK;

	/*
	 * Sanity check
	 */
	if(!wifistack || !mqttparams){

		/*
		 * Error occured
		 */
		status = ERR_FAILURE_STATUS;
	}

	/*
	 * Set the parameters
	 */
	client_if 	= wifistack;
	mqtt_params = mqttparams;

	/*
	 * Create a new PubSubClient
	 */
	iface = new PubSubClient(
		mqttparams->server_name,
		mqttparams->server_port,
		__default_callback,
		*client_if
	);

	/*
	 * Set the status
	 */
	if(!iface){

		/*
		 * Error occured
		 */
		status = STATUS_ERR_DENIED;
	}
}

/**
 * @brief The Connect method for connecting to the mqtt AP
 *
 * This method connects this node to an mqtt server.
 *
 * @return status	The status
 */
mqtt_status_t mqtt::connect(){

	/*
	 * Connect to the server through the ip interface
	 */
	if(!iface->connected()){

		PRINT("Connecting to: "); 	PRINTLN(MQTT_BROKER_ADDR);
		PRINT("With Sensor id: ");	PRINTLN(MQTT_SENSOR_ID);

		/*
		 * Connect
		 */
		if(!iface->connect(mqtt_params->user)){

			/*
			 * Error
			 */
			status = STATUS_ERR_DENIED;
			return MQTT_CONNECT_FAILED;
		}

		PRINT("Connected to: "); 	PRINT(MQTT_BROKER_ADDR);
		PRINT(":");					PRINTLN(MQTT_BROKER_PORT);
	}

    /*
     * Return the status
     */
    return MQTT_CONNECTED;
}

/**
 * @brief The Disconnection mechansim for the wifi conenction.
 *
 * This method disconnects the node from the AP and returns
 * a bool representing the success of the disconnection.
 *
 * @return status	The status
 */
mqtt_status_t mqtt::disconnect(){

	/*
	 * Disconnect
	 */
	iface->disconnect();
	/*
	 * Good disconnect
	 */
	PRINTLN("Disconnected.");
	return MQTT_DISCONNECTED;
}

/**
 * @brief Subscribes to a topic with registered message type and handler.
 *
 * @param topic		The topc to subscribe
 * @param handler	The message type handler to register
 *
 * @return status	The status
 */
mqtt_status_t mqtt::subscribe(mqtt_topic_t topic, mqtt_callback_entry_t* handler){

	/*
	 * Return code
	 */
	mqtt_status_t rc;

	/*
	 * Annouce the subscription
	 */
	PRINT("Subscribing to topic: ");
	PRINTLN(topic);

	/*
	 * Register the callback with the topic
	 */

	if((rc = __register_callback(handler)) != MQTT_SUCCESS_STATUS){

		/*
		 * There was an error in registration.
		 */
		status = STATUS_ERR_DENIED;
		return rc;
	}else{

		/*
		 * Good subscribe
		 */
		PRINTLN("Subscribe successful.");
		return rc;
	}
}

/**
 * @brief Publishes a message to a specified topic.
 *
 * @param mqtt_message 	An mqtt message type
 */
mqtt_status_t mqtt::publish(mqtt_message_t* msg){

	/*
	 * Status
	 */
	mqtt_status_t rc;

	/*
	 * Sanity check
	 */
	if(!msg){

		// Error
		status = STATUS_ERR_DENIED;
		return MQTT_FAILURE_STATUS;
	}

	/*
	 * Publish
	 */
	rc = (mqtt_status_t)iface->publish(
		msg->topic,
		msg->payload,
		msg->length,
		msg->retained
	);

	/*
	 * Check for success
	 */
	if (rc != MQTT_SUCCESS_STATUS) {

		/*
		 * Error occured
		 */
		status = STATUS_ERR_DENIED;

		PRINT("Message publish failed rc: ");
		PRINTLN(rc);
	}
	return rc;
}

/**
 * @brief Runs the whole MQTT stack.
 *
 * @return status		The status
 */
mqtt_status_t mqtt::run(){

	/*
	 * Temp value
	 */
	mqtt_status_t rc;

	/*
	 * All we do in this method is run the stack loop
	 */
	if(!iface->poll()){
		return MQTT_FAILURE_STATUS;
	}
	else{

		/*
		 * Process the payload
		 */
		if((rc = __process()) != MQTT_SUCCESS_STATUS){
			return MQTT_FAILURE_STATUS;
		}
		return MQTT_SUCCESS_STATUS;
	}
}

/**
 * @brief Process a payload from the queue.
 */
mqtt_status_t mqtt::__process(){

	/*
	 * The status
	 */
	mqtt_status_t rc;

	/*
	 * The read message from the queue
	 */
	mqtt_message_t* msg;

	/*
	 * Callback pointer
	 */
	mqtt_callback_entry_t* temp = callbacks;

	/*
	 * Gets the data from the queue and processes it.
	 */
	if(!mqtt_msg_qeue->isEmpty()){

		NOTIFY_INFO("There is a msg to process.");

		/*
		 * Get the messages
		 */
		while(mqtt_msg_qeue->count() > 0){

			/*
			 * Get the message and trigger the callback
			 */
			msg = mqtt_msg_qeue->pop();
			while(temp->next != NULL){

				/*
				 * Check for a match
				 */
				if(strcmp(temp->topic, msg->topic) == COMPARE_SUCCESS){

					/*
					 * We have a match... Now trigger the callback
					 */
					temp->callback(temp->context, msg->payload, msg->length);
					return rc;
				}
			}

			/*
			 * Not a valid command
			 */
			NOTIFY_ERROR("Not a valid command: " + String(msg->topic));
			rc = MQTT_FAILURE_STATUS;
			return rc;
		}
	}
}

/**
 * @brief Registers a new callback interface based on the topic
 *
 * @param entry 		The callback entry provided to register
 */
mqtt_status_t mqtt::__register_callback(mqtt_callback_entry_t* entry){

	/*
	 * Copy the internal pointer to edit
	 */
	mqtt_callback_entry_t* tmp = callbacks;

	/*
	 * Sanity check
	 */
	if(!entry){

		/*
		 * Null pointer
		 */
		status = STATUS_ERR_DENIED;
		return MQTT_FAILURE_STATUS;
	}

	/*
	 * Cycle to the end of the list
	 */
	while(tmp->next != NULL){
		tmp = tmp->next;
	}

	/*
	 * Create a new entry struct to enter the list.
	 * We are now at the end of the list.
	 */
	tmp->next = (mqtt_callback_entry_t*)malloc(sizeof(mqtt_callback_entry_t));

	/*
	 * Copy over the new entry
	 */
	memcpy(tmp->next, entry, sizeof(mqtt_callback_entry_t));

	status = STATUS_ERR_DENIED;
	return MQTT_SUCCESS_STATUS;
}


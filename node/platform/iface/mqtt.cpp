/*
 * mqtt.cpp
 *
 *  Created on: Jul 4, 2015
 *      Author: francis-ccs
 */

#include "mqtt.h"

/**
 * @brief The MQTT Interface Constructor
 *
 * This is the entry method for the MQTT interface object.
 * We must pass the stack instance.
 *
 * @param ipstack		The stack instance
 */
mqtt::mqtt(WifiIPStack* ipstack){

	/*
	 * All is good
	 */
	status = STATUS_OK;

	/*
	 * Set the interanl reference
	 */
	stack_if = ipstack;

	/*
	 * Create the client
	 */
	client_if = new \
			MQTT::Client<WifiIPStack, Countdown, MQTT_MAX_PACKET_SIZE> (*stack_if);

	/*
	 * Set the status
	 */
	if(!stack_if){

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
 * @param address	The address to connect to
 * @param port 		The port to connect to
 *
 * @return status	The status
 */
mqtt_status_t mqtt::connect(mqtt_broker_t address, mqtt_port_t port){

	/*
	 * Temp status
	 */
	wl_status_t temp_status;
	int rc = -1;

	/*
	 * Connect to the server through the ip interface
	 */
	if(!client_if->isConnected()){

		PRINT("Connecting to: "); 	PRINTLN(MQTT_BROKER_ADDR);
		PRINT("With Sensor id: ");	PRINTLN(MQTT_SENSOR_ID);

		/*
		 * Connect
		 */
		while((temp_status = (wl_status_t)\
				stack_if->connect(MQTT_BROKER_ADDR, \
						MQTT_BROKER_PORT)) != WL_CONNECTED){


			/*
			 * Check the return code to see if its a valid error.
			 */
			switch (temp_status){

				/*
				 * Connection Failed
				 */
				case WL_CONNECT_FAILED:
				case WL_CONNECTION_LOST:
				case WL_AP_MODE:
				case WL_NO_SSID_AVAIL:
				default:
					status = STATUS_ERR_DENIED;
					return MQTT_CONNECT_FAILED;
				}
		}

		PRINT("Connected to: "); 	PRINT(MQTT_BROKER_ADDR);
		PRINT(":");					PRINTLN(MQTT_BROKER_PORT);
	}

	/*
	 * Craft the connect request
	 */
    MQTTPacket_connectData options 	= \
    		MQTTPacket_connectData_initializer;

    options.MQTTVersion 			= MQTT_VERSION;
    options.clientID.cstring 		= MQTT_SENSOR_ID_STR;
    options.username.cstring 		= MQTT_USERNAME;
    options.password.cstring 		= MQTT_PASSWORD;
    options.keepAliveInterval 		= MQTT_KEEP_ALIVE;

	/*
	 * Connect the the mqtt broker
	 */
    while ((rc = client_if->connect(options)) != MQTT_SUCCESS_STATUS);
    PRINTLN("Connected to the broker.");

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
	if(client_if->disconnect() != MQTT_SUCCESS_STATUS){

		/*
		 * Issue
		 */
		status = STATUS_ERR_DENIED;
		return MQTT_DISCONNECT_FAILED;
	}else{

		/*
		 * Good disconnect
		 */
		PRINTLN("Disconnected.");
		return MQTT_DISCONNECTED;
	}
}

/**
 * @brief Subscribes to a topic with registered message type and handler.
 *
 * @param topic		The topc to subscribe
 * @param handler	The message type handler to register
 *
 * @return status	The status
 */
mqtt_status_t mqtt::subscribe(mqtt_topic_t topic, mqtt_handler_t handler){

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
	 * Make sure its not already registered
	 */
	if((rc = \
			unsubscribe(topic)) != MQTT_SUCCESS_STATUS){

		/*
		 * Error occured
		 */
		status = STATUS_ERR_DENIED;
		return rc;
	}

	/*
	 * Subscribe
	 */
	if((rc = \
			(mqtt_status_t)client_if->subscribe(topic, MQTT::QOS0, handler)) \
			!= MQTT_SUCCESS_STATUS){

		/*
		 * Error occured
		 */
		status = STATUS_ERR_DENIED;

		/*
		 * Issue subscribing
		 */
		PRINT("Subscribtion failed with rc: ");
		PRINTLN(rc);

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
 * @brief Unsubscribe a topic
 *
 * @param topic		The topic to unsubscribe.
 * @return status	The status code
 */
mqtt_status_t mqtt::unsubscribe(mqtt_topic_t topic){

	/*
	 * Unsubscribe
	 */
	return (mqtt_status_t)unsubscribe(topic);
}

/**
 * @brief Publishes a message to a specified topic.
 *
 * @param topic		The topic to publich to
 * @param message	The message to publish
 * @return status	The status code
 */
mqtt_status_t mqtt::publish(mqtt_topic_t topic, MQTT::Message* message){

	/*
	 * Status
	 */
	mqtt_status_t rc;

	/*
	 * Setup the message
	 */
	message->qos 		= MQTT::QOS0;
	message->retained 	= false;

	/*
	 * Publish
	 */
	rc = (mqtt_status_t)client_if->publish((const char*)topic, *message);

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
 * @brief Gets the wifi internal stack handle.
 *
 * @return handle	The wifi handler stack handle.
 */
WifiIPStack* mqtt::get_stack(){
	return stack_if;
}

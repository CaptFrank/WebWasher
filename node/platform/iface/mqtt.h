/*
 * mqtt.h
 *
 *  Created on: Jul 4, 2015
 *      Author: francis-ccs
 */

#ifndef SERVICES_COMMS_MQTT_H_
#define SERVICES_COMMS_MQTT_H_

#include <configs.h>
#include <service/services.h>
#include <status_codes.h>

#include <Countdown.h>
#include <MQTTClient.h>

/*
 * Maximum registered callbacks
 */
#define MQTT_MAX_CALLBACKS		255

/**
 * @brief Type definitions
 */
typedef int 	mqtt_port_t;
typedef char*	mqtt_broker_t;
typedef char* 	mqtt_topic_t;

/**
 * @brief The valid return codes
 */
typedef enum {
	BUFFER_OVERFLOW = -2,
	FAILURE = -1,
	SUCCESS = 0
}mqtt_rc_t;

/**
 * @brief Callback function registration
 */
typedef void (*mqtt_handler_t)(MQTT::MessageData &md);

/**
 * @brief MQTT Interface Class
 *
 * This is the interface that must be called in order to access
 * the wifi IP stack and to communicate over MQTT.
 */

class mqtt {

	/*
	 * Private access attributes
	 */
	private:

		/*
		 * The stack interface
		 */
		WifiIPStack* 					stack_if;

		/*
		 * The MQTT client interface
		 */
		MQTT::Client <WifiIPStack, Countdown, MQTT_MAX_PACKET_SIZE>* \
										client_if;

		/*
		 * The class status
		 */
		status_code_t 					status;

	/*
	 * Public access methods
	 */
	public:

		/**
		 * @brief The MQTT Interface Constructor
		 *
		 * This is the entry method for the MQTT interface object.
		 * We must pass the stack instance.
		 *
		 * @param ipstack		The stack instance
		 */
		mqtt(WifiIPStack* ipstack);

		/**
		 * @brief The Default Deconstructor for the class.
		 */
		~mqtt(){};

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
		mqtt_status_t connect(mqtt_broker_t address, mqtt_port_t port);

		/**
		 * @brief The Disconnection mechansim for the wifi conenction.
		 *
		 * This method disconnects the node from the AP and returns
		 * a bool representing the success of the disconnection.
		 *
		 * @return status	The status
		 */
		mqtt_status_t disconnect();

		/**
		 * @brief Subscribes to a topic with registered message type and handler.
		 *
		 * @param topic		The topc to subscribe
		 * @param handler	The message type handler to register
		 *
		 * @return status	The status
		 */
		mqtt_status_t subscribe(mqtt_topic_t topic, mqtt_handler_t handler);

		/**
		 * @brief Unsubscribe a topic
		 *
		 * @param topic		The topic to unsubscribe.
		 * @return status	The status code
		 */
		mqtt_status_t unsubscribe(mqtt_topic_t topic);

		/**
		 * @brief Publishes a message to a specified topic.
		 *
		 * @param topic		The topic to publich to
		 * @param message	The message to publish
		 * @return status	The status code
		 */
		mqtt_status_t publish(mqtt_topic_t topic, MQTT::Message* message);

	    /**
	     * @brief Gets the wifi internal stack handle.
	     *
	     * @return handle	The wifi handler stack handle.
	     */
	    WifiIPStack* get_stack();

	    /**
	     * @brief Gets the internal status of the driver.
	     *
	     * @return status	The status
	     */
	    status_code_t get_status(){
	    	return status;
	    }

};

/**< Typedef */
typedef mqtt mqtt_t;

#endif /* SERVICES_COMMS_MQTT_H_ */

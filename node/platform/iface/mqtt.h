/*
 * mqtt.h
 *
 *  Created on: Jul 4, 2015
 *      Author: francis-ccs
 */

#ifndef SERVICES_COMMS_MQTT_H_
#define SERVICES_COMMS_MQTT_H_

#include <configs.h>
#include <status_codes.h>

#include <Wifi.h>
#include <Strings.h>
#include <PubSubClient.h>

/*
 * Maximum registered callbacks
 */
#define MQTT_MAX_CALLBACKS			(255)
#define MQTT_DELAY					(100) // Delay 1 sec

/*
 * Local defines
 */
#define TOPIC_DELIMITER				("/")
#define COMPARE_SUCCESS 			(0x00)

/**
 * @brief Type definitions
 */
typedef uint16_t	mqtt_port_t;
typedef char*		mqtt_broker_t;
typedef char* 		mqtt_topic_t;
typedef char*		mqtt_user_t;
typedef void*		mqtt_args_t;
typedef void* 		context_t;

/**
 * This is the message type
 */
typedef struct {

	mqtt_topic_t 	topic;		//!< The topic
	byte* 			payload;	//!< The payload in bytes
	unsigned int 	length;		//!< The payload length
	boolean 		retained;	//!< Retain payload
}mqtt_message_t;

/**
 * @brief The valid return codes
 */
typedef enum {
	BUFFER_OVERFLOW = -2,
	FAILURE = -1,
	SUCCESS = 0
}mqtt_rc_t;


/**
 * This is the callback definition needed for the publish/subscribe
 * enigne. (i.e. MQTT)
 */
typedef void (*mqtt_callback_t)(mqtt_topic_t, byte*, unsigned int);

/**
 * This is the command prototype that will trigger on the payload.
 */
typedef mqtt_status_t (*mqtt_action_t)(context_t context, void* payload, uint16_t length);

/**
 * This is the mqtt parameters structure.
 * It defines the necessary pointers and attributes.s
 */
typedef struct {

	mqtt_user_t 	user;				//!< The connection user
	mqtt_broker_t 	server_name;		//!< The server name
	mqtt_port_t 	server_port;		//!< The server port
}mqtt_params_t;

/**
 * The callback entry that will be used as a linked list to
 * link each callback.
 */
typedef struct mqtt_callback_entry_t{

	mqtt_topic_t 					topic;				//!< The topic that is the anchor
	context_t						context;			//!< The operating context
	mqtt_action_t 					callback;			//!< The command
	struct mqtt_callback_entry_t* 	next;				//!< the next command
}mqtt_callback_entry_t;

/**
 * @breif The Default callback for the MQTT engine.
 *
 * @param topic		The topic subscribed to
 * @param payload	The payload to read
 * @param length	The length
 */
extern void __default_callback(char* topic, byte* payload, unsigned int length);

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
		 * The class status
		 */
		status_code_t 					status;

		/*
		 * Wifi stack client
		 */
		Client* 						client_if;

		/*
		 * MQTT parameters
		 */
		mqtt_params_t*					mqtt_params;

		/*
		 * PubSubClient
		 */
		PubSubClient*					iface;

		/*
		 * The callback list
		 */
		mqtt_callback_entry_t*			callbacks;

		/**
		 * @brief Registers a new callback interface based on the topic
		 *
		 * @param entry 		The callback entry provided to register
		 */
		mqtt_status_t __register_callback(mqtt_callback_entry_t* entry);

		/**
		 * @brief Process a payload from the queue.
		 */
		mqtt_status_t __process();

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
		 * @param wifistack		The stack instance
		 * @param mqttparams	The mqtt parameters
		 */
		mqtt(Client* wifistack, mqtt_params_t* mqttparams);

		/**
		 * @brief The Default Deconstructor for the class.
		 */
		~mqtt(){};

		/**
		 * @brief The Connect method for connecting to the mqtt AP
		 *
		 * This method connects this node to an mqtt server.
		 *
		 * @return status		The status
		 */
		mqtt_status_t connect();

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
		 * @param topic			The topc to subscribe
		 * @param handler		The message type handler to register
		 *
		 * @return status		The status
		 */
		mqtt_status_t subscribe(mqtt_topic_t topic, mqtt_callback_entry_t* handler);

		/**
		 * @brief Publishes a message to a specified topic.
		 *
		 * @param mqtt_message 	An mqtt message type
		 */
		mqtt_status_t publish(mqtt_message_t* msg);

		/**
		 * @brief Runs the whole MQTT stack.
		 *
		 * @return status		The status
		 */
		mqtt_status_t run();

	    /**
	     * @brief Gets the internal status of the driver.
	     *
	     * @return status		The status
	     */
	    status_code_t get_status(){
	    	return status;
	    }

};

/**< Typedef */
typedef mqtt mqtt_t;

#endif /* SERVICES_COMMS_MQTT_H_ */

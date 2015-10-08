/*
 * coms.h
 *
 *  Created on: Jul 30, 2015
 *      Author: fpapinea
 */

#ifndef SERVICE_SERVICES_COMS_H_
#define SERVICE_SERVICES_COMS_H_

#include <string.h>
#include <Countdown.h>
#include <MQTTClient.h>
#include <status_codes.h>

#include <task/task.h>
#include <platform/iface/mqtt.h>
#include <platform/iface/wifi.h>

#include <service/services/formatter/formatter.h>
#include <service/services/system/system.h>

/*
 * Local Defines
 */
#define TOPIC_DELIMITER				("/")
#define COMPARE_SUCCESS 			(0x00)
#define COMS_SERVICE_DELAY			(1000) // Delay 1 sec

#define ARGUMENT_ERROR				("ARGUMENT ERROR")
#define ARGUMENT_ERROR_LEN			(strlen(ARGUMENT_ERROR))

/**
 * @brief The remote commands allowed
 */
typedef enum {

	COMMAND_TYPE_DISCONNECT,//!< COMMAND_TYPE_DISCONNECT
	COMMAND_TYPE_REBOOT,    //!< COMMAND_TYPE_REBOOT
	COMMAND_TYPE_STATUS,    //!< COMMAND_TYPE_STATUS
	COMMAND_TYPE_RESTART,   //!< COMMAND_TYPE_RESTART
	COMMAND_TYPE_SUSPEND,   //!< COMMAND_TYPE_SUSPEND
	COMMAND_TYPE_SELFTEST,  //!< COMMAND_TYPE_SELFTEST
	COMMAND_TYPE_GET,       //!< COMMAND_TYPE_GET
	COMMAND_TYPE_ECHO,		//!< COMMAND_TYPE_ECHO

	COMMAND_TYPE_MAX_SIZE   //!< COMMAND_TYPE_MAX_SIZE
}command_t;


/**
 * @brief The command map definiton
 */
typedef struct {

	char* 		topic;		// The topic to test
	command_t 	type;		// The command type
}command_map_t;

/**
 * @brief The interface descriptors
 */
typedef enum {

	INTERFACE_WIFI,			//!< INTERFACE_WIFI
	INTERFACE_MQTT,			//!< INTERFACE_MQTT
	INTERFACE_BOTH 			//!< INTERFACE_BOTH
}interface_t;

/**
 * @brief The communication service
 *
 * This is the high level wrapper class interfacing the remote
 * communications to the device. It handles the higher level
 * application controls to the drivers.
 *
 * @extends service_t
 */
class coms_svr {

	/*
	 * Public method access
	 */
	public:

		/**
		 * @brief The default constructor for the comms object
		 *
		 * This is where we initialize the interfaces; both the
		 * MQTT interface and the underlying wifi interface.
		 */
		coms_svr();

		/**
		 * @brief The default deconstructor for the comms object.
		 */
		~coms_svr(){};

		/**
		 * @brief Connects to the wifi hotspot and to the mqtt broker.
		 *
		 * The default entry point to connect both to the wifi hotspot and
		 * to the mqtt broker.
		 *
		 * @param 	iface			The index of the interface to operate on
		 * @return 	status			The status of the operation
		 */
		status_code_t connect(interface_t iface);

		/**
		 * @brief Disconnects from the wifi hotspot and the mqtt broker.
		 *
		 * The default entry point to connect both to the wifi hotspot and
		 * to the mqtt broker.
		 *
		 * @param 	iface			The index of the interface to operate on
		 * @return 	status			The status of the operation
		 */
		status_code_t disconnect(interface_t iface);

		/**
		 * @brief Sends a message to the broker
		 *
		 * This sends a message type to the mqtt broker or to the receiving server.
		 *
		 * @param 	msg_type		The message type
		 * @param 	msg				The message data
		 * @return 	status			The status of the operation
		 */
		mqtt_status_t send(msg_type_t msg_type, msg_t* msg);

		/**
		 * @brief Processes a request
		 *
		 * This processes a received message from the mqtt borker.
		 *
		 * @param 	command			The command type received
		 * @param	msg				The message data
		 * @return 	status			The status of the operation
		 */
		status_code_t process(command_t command, msg_t* msg);

		/**
		 * @brief The callback for a subscription
		 *
		 * This is the callback for the topic subscription.
		 * It is invoked when there is a message that corresponds
		 * to a topic specified is received.
		 *
		 * @param md				The message descriptor
		 */
		static void coms_callback(MQTT::MessageData& md);

	/*
	 * public attrbute access
	 */
	public:

		/* ===========================
		 * Public Attributes
		 */

		/*
		 * Comms interfaces
		 */
		wifi_t* 			wifi_if;
		mqtt_t* 			mqtt_if;

		/* ===========================
		 * Comms interface attributes
		 */

		/*
		 * Wifi
		 */
		WifiIPStack* 		ip_stack;
		wifi_attributes_t 	wifi_attr;
};

typedef coms_svr coms_t;

/**
 * MQTT Callback function
 */

/**
 * @brief The default callback function for the MQTT engine.
 *
 * 	This method is called when there is a new message that is received
 * 	from the mqtt interface.
 *
 * @param topic			The topic that the message is received in
 * @param payload		The actual payload of the message
 * @param length		The length of the message
 */
extern void callback(char* topic, byte* payload, unsigned int length);

#endif /* SERVICE_SERVICES_COMS_H_ */

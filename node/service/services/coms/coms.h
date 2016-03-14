/*
 * coms.h
 *
 *  Created on: Jul 30, 2015
 *      Author: fpapinea
 */

#ifndef SERVICE_SERVICES_COMS_H_
#define SERVICE_SERVICES_COMS_H_

#include <WiFi.h>
#include <string.h>
#include <PubSubClient.h>
#include <status_codes.h>

#include <platform/iface/wifi.h>
#include <platform/iface/mqtt.h>

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
 * This is the union that defines the
 * packet and the construction mecanism.
 */
typedef union {

	/*
	 * The struct that contains both the command
	 * and the arguments.
	 *
	 * [PACKET [ COMMAND ] [ ARGUMENT ] ]
	 */
	struct {
		uint8_t command;
		uint8_t argument;
	}components;

	/*
	 * The packet.s
	 */
	uint16_t packet;

}packet_t;

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
class coms_service {

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
		coms_service();

		/**
		 * @brief The default deconstructor for the comms object.
		 */
		~coms_service(){};

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
		 * @brief Processes a local request
		 *
		 * This processes a received message from the mqtt borker.
		 *
		 * @param obj		The object to use
		 * @param payload	The payload array
		 * @param length 	The payload length
		 * @return status	The status of the operation
		 */
		static status_code_t __process_local(void* obj, void* payload, uint16_t length);

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
		mqtt_t* 			mqtt_if;
		mqtt_params_t		mqtt_params;

		/* ===========================
		 * Comms interface attributes
		 */

		/*
		 * Wifi
		 */
		WiFiClient 			wifi_stack;

		wifi_t*				wifi_if;
		wifi_attributes_t 	wifi_attrs;
};

typedef coms_service coms_t;

#endif /* SERVICE_SERVICES_COMS_H_ */

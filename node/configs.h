/*
 * configs.h
 *
 *  Created on: Jul 27, 2015
 *      Author: fpapinea
 */

#ifndef CONFIGS_H_
#define CONFIGS_H_

#include "Energia.h"

/**
 * Cache types
 */
typedef enum {

	CACHE_TYPE_HEARTBEAT	= 0x0600,	//!< CACHE_TYPE_HEARTBEAT
	CACHE_TYPE_STATUS		= 0x0400,	//!< CACHE_TYPE_STATUS
	CACHE_TYPE_TEMP_DATA	= 0x0200,	//!< CACHE_TYPE_TEMP_DATA
	CACHE_TYPE_ACC_DATA  	= 0x0001,	//!< CACHE_TYPE_ACC_DATA
	CACHE_TYPE_ALL			= 0x0000,	//!< CACHE_TYPE_ALL
}cache_t;

/**
 * @brief The message types allowed
 */
typedef enum {
	MSG_TYPE_HEARTBEAT,		//!< MSG_TYPE_HEARTBEAT
	MSG_TYPE_STATUS,   		//!< MSG_TYPE_STATUS
	MSG_TYPE_TEMP_DATA,		//!< MSG_TYPE_TEMP_DATA
	MSG_TYPE_ACC_DATA, 		//!< MSG_TYPE_ACC_DATA
	MSG_TYPE_OTHER			//!< MSG_TYPE_OTHER
}msg_type_t;

/*
 * Local Typedefs
 */
typedef uint16_t msg_length_t;
typedef void* msg_data_t;


/**
 * @brief The message type definition
 */
typedef struct {

	msg_length_t 	length;	// The message length
	msg_data_t		data;	// The message data
}msg_t;


/**
 * @brief Easier Macros to use
 */
#define PRINT					Serial.print
#define PRINTLN					Serial.println

#define ERROR					String("<[ERROR]:")
#define INFO					String("<[INFO]:")


#define NOTIFY_ERROR(var) 		Serial.println(ERROR 	+ String(var))
#define NOTIFY_INFO(var) 		Serial.println(INFO 	+ String(var))

#define HEARTBEAT_LED			RED_LED
#define RECEIVE_LED				YELLOW_LED
#define STATUS_LED				GREEN_LED

#define DEVICE_VERSION			0x01
#define DEVICE_ID				0x01

#define NUMBER_OF_SENSORS		2
#define DEFAULT_DELAY			100 // 100ms

/*
 * Tasks to be enabled
 */
#define DAQ_TASK_ENABLE
#define UPDATE_TASK_ENABLE
#define PUBLISH_TASK_ENABLE
#define IDLE_TASK_ENABLE

#define TASK_NUMBER				4

/**
 * @brief WIFI definitons
 */

/**
 * @brief Wifi authentication
 */
#define WIFI_SSID				("Haligonia Sensor Network")
#define WIFI_PASS				("@Sensor!1!haligonia@")

#define WIFI_LOCAL_IP			{0x0A, 0x00, 0x01, 0xC8} // 10.0.1.200
#define WIFI_LOCAL_IP_STR		("10.0.1.200")

#define WIFI_LOCAL_DNS			{0x0A, 0x00, 0x01, 0x11} // 10.0.1.17
#define WIFI_LOCAL_DNS_STR		("10.0.1.17")

#define WIFI_LOCAL_GATEWAY		{0x0A, 0x00, 0x01, 0x01} // 10.0.1.1
#define WIFI_LOCAL_GATEWAY_STR	("10.0.1.1")

#define WIFI_LOCAL_SUBNET		{0xFF, 0xFF, 0xFF, 0x00} // 255.255.255.0
#define WIFI_LOCAL_SUBNET_STR	("255.255.255.0")

#define WIFI_DEFAULT_TIMEOUT	1000

/**
 * @brief MQTT Definitions.
 */

/*!
 * Maximum MQTT packet length
 */
#define MQTT_MAX_PACKET_SIZE 		(255)

/*!
 * Publish
 */
#define MQTT_PUBLISH_DATA_TEMP		("sensor/01/data/temp")
#define MQTT_PUBLISH_DATA_ACC		("sensor/01/data/acc")
#define MQTT_PUBLISH_STATUS			("sensor/01/status")

/*!
 * Subscribe
 * 	- global_cmd
 * 		- disconnect				- one byte -- interface desc.
 * 		- reboot					- one byte -- service/driver desc.
 * 		- status					- one byte -- true/false
 * 		- restart					- one byte -- true/false
 * 		- suspend
 * 			- thread id				- one byte -- thread id
 * 			- all == 255
 * 		- selftest					- one byte -- true/false
 * 		- get						- one byte -- data type
 */
#define MQTT_SUB_TOPIC_1			("global_cmd/#")
#define MQTT_SUB_TOPIC_2			("sensor_cmd/01/#")

/**
 * Commands
 */
#define MQTT_SUB_COMMAND_REBOOT		("reboot")
#define MQTT_SUB_COMMAND_RESTART	("restart")
#define MQTT_SUB_COMMAND_SUSPEND	("suspend")
#define MQTT_SUB_COMMAND_DISCONNECT	("disconnect")

#define MQTT_SUB_COMMAND_STATUS		("status")
#define MQTT_SUB_COMMAND_SELFTEST	("selftest")

#define MQTT_SUB_COMMAND_GET		("get")

/**
 * @brief MQTT Broker attributes
 */
#define MQTT_BROKER_ADDR			("broker.haligonia.home.com")
#define MQTT_BROKER_PORT			(1883)

/**
 * @brief MQTT Attributes
 */
#define MQTT_VERSION				(3)
#define MQTT_SENSOR_ID				(1)
#define MQTT_USERNAME				("use-token-auth")
#define MQTT_PASSWORD				("haligonia")
#define MQTT_KEEP_ALIVE				(10)

#endif /* CONFIGS_H_ */

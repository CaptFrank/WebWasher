/*
 * formatter.cpp
 *
 *  Created on: Aug 6, 2015
 *      Author: fpapinea
 */

#include <service/services/formatter/formatter.h>
#include "json.h"


/**
 * @brief String table
 */
static const string_table_t string_table[] = {

		{
				CACHE_TYPE_ACC_DATA, 	formatter_t::acc_json
		},
		{
				CACHE_TYPE_TEMP_DATA, 	formatter_t::temp_json
		},
		{
				CACHE_TYPE_HEARTBEAT, 	formatter_t::heartbeat_json
		},
		{
				CACHE_TYPE_STATUS, 		formatter_t::status_json
		},
};

/**
 * @brief Default constructor
 */
formatter::formatter(){

	/*
	 * Setup the values that do not change
	 */

	/*
	 * Get the cache
	 */
	status_cache_t* status_cache = \
		(status_cache_t*)system_t::BIOS_cache(CACHE_TYPE_STATUS);

	/*
	 * Device
	 */
	status_cache->device_data.version									= DEVICE_VERSION;
	status_cache->device_data.id										= DEVICE_ID;

	/*
	 * MQTT
	 */
	status_cache->coms_data.mqtt_data.broker.string						= MQTT_BROKER_ADDR;
	status_cache->coms_data.mqtt_data.port								= MQTT_BROKER_PORT;
	status_cache->coms_data.mqtt_data.ver								= MQTT_VERSION;
	status_cache->coms_data.mqtt_data.id								= MQTT_SENSOR_ID;
	status_cache->coms_data.mqtt_data.topics.pub.temp.string			= MQTT_PUBLISH_DATA_TEMP;
	status_cache->coms_data.mqtt_data.topics.pub.acc.string				= MQTT_PUBLISH_DATA_ACC;
	status_cache->coms_data.mqtt_data.topics.pub.status.string			= MQTT_PUBLISH_STATUS;
	status_cache->coms_data.mqtt_data.topics.sub.glob.string			= MQTT_SUB_TOPIC_1;
	status_cache->coms_data.mqtt_data.topics.sub.glob.len				= strlen(MQTT_SUB_TOPIC_1);
	status_cache->coms_data.mqtt_data.topics.sub.local.string			= MQTT_SUB_TOPIC_2;
	status_cache->coms_data.mqtt_data.topics.sub.local.len				= strlen(MQTT_SUB_TOPIC_2);

	/*
	 * IP
	 */
	status_cache->coms_data.ip_data.timeout								= WIFI_DEFAULT_TIMEOUT;
	status_cache->coms_data.ip_data.ssid.string							= WIFI_SSID;
	status_cache->coms_data.ip_data.ip.string							= WIFI_LOCAL_IP_STR;
	status_cache->coms_data.ip_data.gw.string	 						= WIFI_LOCAL_GATEWAY_STR;
	status_cache->coms_data.ip_data.dns.string							= WIFI_LOCAL_DNS_STR;
	status_cache->coms_data.ip_data.subnet.string						= WIFI_LOCAL_SUBNET_STR;
}


/**
 * @brief The cache type to format into a string
 *
 * @param 	type			The cache type to address
 * @return	the string		The string formatted
 */
msg_t* formatter::format(cache_t type){

	// Container
	bma222_cache_t* acc_cache;
	tmp006_cache_t* temp_cache;
	heartbeat_cache_t* heart_cache;
	status_cache_t* status_cache;

	/*
	 * Get the time
	 */
	char* time = formatter::convert_time(millis());

	/*
	 * Switch on the cache type
	 */
	switch(type){

		/*
		 * Accelerometer cache update
		 */
		case CACHE_TYPE_ACC_DATA:
		{

			/*
			 * Get the cache
			 */
			acc_cache = \
				(bma222_cache_t*)system_t::BIOS_cache(CACHE_TYPE_ACC_DATA);

			/*
			 * Format
			 */
			sprintf(
					string_table[type].string,
					MQTT_ACC_JSON,
					time,
					acc_cache->temp.temperature.value,
					acc_cache->acc.acc.axis.x,
					acc_cache->acc.acc.axis.y,
					acc_cache->acc.acc.axis.z
					);
		}break;

		/*
		 * Temperature cache update
		 */
		case CACHE_TYPE_TEMP_DATA:
		{
			/*
			 * Get the cache
			 */
			temp_cache = \
				(tmp006_cache_t*)system_t::BIOS_cache(CACHE_TYPE_TEMP_DATA);

			/*
			 * Format
			 */
			sprintf(
					string_table[type].string,
					MQTT_TEMP_JSON,
					time,
					temp_cache->temps.obj_temp,
					temp_cache->temps.die_temp,
					temp_cache->voltage.voltage.value
					);
		}break;

		/*
		 * Heartbeat cache update
		 */
		case CACHE_TYPE_HEARTBEAT:
		{
			/*
			 * Get the cache
			 */
			heart_cache = \
				(heartbeat_cache_t*)system_t::BIOS_cache(CACHE_TYPE_HEARTBEAT);

			/*
			 * Format
			 */
			sprintf(
					string_table[type].string,
					MQTT_HEARTBEAT_JSON,
					time,
					heart_cache->heartbeat_data.alive,
					heart_cache->heartbeat_data.state
					);
		}break;

		/*
		 * Status cache update
		 */
		case CACHE_TYPE_STATUS:
		{

			/*
			 * Get the cache
			 */
			status_cache = \
				(status_cache_t*)system_t::BIOS_cache(CACHE_TYPE_STATUS);

			/*
			 * Format
			 */
			sprintf(
					string_table[type].string,
					MQTT_STATUS_JSON,
					time,

					// Device
					status_cache->device_data.status,
					status_cache->device_data.alive,
					status_cache->device_data.runtime,
					status_cache->device_data.version,
					status_cache->device_data.id,

					// Coms

					// MQTT
					status_cache->coms_data.mqtt_data.status,
					status_cache->coms_data.mqtt_data.broker.string,
					status_cache->coms_data.mqtt_data.port,
					status_cache->coms_data.mqtt_data.ver,
					status_cache->coms_data.mqtt_data.id,
					status_cache->coms_data.mqtt_data.topics.pub.temp.string,
					status_cache->coms_data.mqtt_data.topics.pub.acc.string,
					status_cache->coms_data.mqtt_data.topics.pub.status.string,
					status_cache->coms_data.mqtt_data.topics.sub.glob,
					status_cache->coms_data.mqtt_data.topics.sub.local,

					// IP
					status_cache->coms_data.ip_data.status,
					status_cache->coms_data.ip_data.socket,
					status_cache->coms_data.ip_data.ip,
					status_cache->coms_data.ip_data.timeout,
					status_cache->coms_data.ip_data.ssid.string,
					status_cache->coms_data.ip_data.gw.string,
					status_cache->coms_data.ip_data.dns.string,
					status_cache->coms_data.ip_data.subnet.string
					);
		}break;

		/*
		 * No cache update
		 */
		default:
			return NULL;
	}

	/*
	 * Create the string
	 */
	msg->data 	= string_table[type].string;
	msg->length 	= strlen(string_table[type].string);
	return msg;
}

/**
 * @brief Converts the unsigned long time that is converted to
 * a time string.
 *
 * Converting the unsinged long value to a string value that is
 * formatted to a conventional time string (i.e. xx:xx:xx)
 *
 * @param 	time			The unsigned long time value
 * @return 	string			The time formatted to the input.
 */
char* formatter::convert_time(uint32_t time){

	/*
	 * Temp containers
	 */
	char* string;

	int days, hours, minutes;

	/*
	 * Get the time
	 */
	days 			= time / (60 * 60 * 24);
	time 			-= days * (60 * 60 * 24);
	hours 			= time / (60 * 60);
	time 			-= hours * (60 * 60);
	minutes 		= time / 60;

	sprintf(string, "%d:%d:%d", hours, minutes, time);
	return string;
}

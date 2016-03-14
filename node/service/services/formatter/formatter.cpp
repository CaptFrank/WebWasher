/*
 * formatter.cpp
 *
 *  Created on: Aug 6, 2015
 *      Author: fpapinea
 */

#include <service/services/formatter/formatter.h>
#include <service/services/system/system.h>
#include "json.h"


/*
 * Strings
 */
static char acc_json			[50];
static char temp_json			[50];
static char heartbeat_json		[50];
static char status_json			[500];

using namespace system_base;

/**
 * @brief String table
 */
static const string_table_t string_table[] = {

		{
				CACHE_TYPE_ACC_DATA, 	acc_json
		},
		{
				CACHE_TYPE_TEMP_DATA, 	temp_json
		},
		{
				CACHE_TYPE_HEARTBEAT, 	heartbeat_json
		},
		{
				CACHE_TYPE_STATUS, 		status_json
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
		(status_cache_t*)system_base::BIOS_cache(CACHE_TYPE_STATUS);

	/*
	 * Device
	 */
	status_cache->device_data.version									= DEVICE_VERSION;
	status_cache->device_data.id										= DEVICE_ID;
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

	// Message type
	msg_t msg;

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
				(bma222_cache_t*)system_base::BIOS_cache(CACHE_TYPE_ACC_DATA);

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
				(tmp006_cache_t*)system_base::BIOS_cache(CACHE_TYPE_TEMP_DATA);

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
				(heartbeat_cache_t*)system_base::BIOS_cache(CACHE_TYPE_HEARTBEAT);

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
				(status_cache_t*)system_base::BIOS_cache(CACHE_TYPE_STATUS);

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
					status_cache->device_data.id
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
	msg.data 		= string_table[type].string;
	msg.length 		= strlen(string_table[type].string);
	return &msg;
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

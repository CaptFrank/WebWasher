/*
 * caches.h
 *
 *  Created on: Aug 5, 2015
 *      Author: fpapinea
 */

#ifndef PLATFORM_CACHES_H_
#define PLATFORM_CACHES_H_

/**
 * @brief Define the string type
 */
typedef struct {

	char* string;
	uint16_t len;
}string_t;

/**
 * @brief Define the hearbeat data structure
 */
typedef struct {

	struct {
		bool 					alive;
		uint8_t 				state;
	}heartbeat_data;
}heartbeat_cache_t;

/**
 * @brief Define the status data structure
 */
typedef struct {

	struct {
		uint8_t 				status;
		bool 					alive;
		uint32_t 				runtime;
		uint8_t 				version;
		uint8_t					id;
	}device_data;

	struct {
		struct{
			uint8_t 			status;
			string_t			broker;
			uint16_t			port;
			uint8_t				ver;
			uint8_t				id;

			struct {
				struct {
					string_t 	temp;
					string_t 	acc;
					string_t 	status;
				}pub;
				struct{
					string_t 	glob;
					string_t 	local;
				}sub;
			}topics;
		}mqtt_data;

		struct{
			uint8_t 			status;
			uint8_t 			socket;
			string_t			ip;
			uint16_t 			timeout;
			string_t			ssid;
			string_t			gw;
			string_t 			dns;
			string_t			subnet;
		}ip_data;
	}coms_data;
}status_cache_t;


#endif /* PLATFORM_CACHES_H_ */

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
}status_cache_t;


#endif /* PLATFORM_CACHES_H_ */

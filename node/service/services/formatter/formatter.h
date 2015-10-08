/*
 * formatter.h
 *
 *  Created on: Aug 6, 2015
 *      Author: fpapinea
 */

#ifndef SERVICE_SERVICES_FORMATTER_FORMATTER_H_
#define SERVICE_SERVICES_FORMATTER_FORMATTER_H_

#include <time.h>
#include <configs.h>
#include <platform/platform.h>
#include <service/services/system/system.h>

#include "strings.h"

/**
 * @brief String table entry
 */
typedef struct {

	cache_t 	type;
	char* 		string;
}string_table_t;

/**
 * @brief The formatter interface.
 *
 * This static class is the interface to the formatting
 * of the json strings and cache structures updates with the
 * most pertinent data.
 */
class formatter {

	/*
	 * The Public Access mehods
	 */
	public:

		/*
		 * Strings
		 */
		static char acc_json			[50];
		static char temp_json			[50];
		static char heartbeat_json		[50];
		static char status_json			[500];

		static msg_t* msg;

		/**
		 * @brief Default constructor
		 */
		formatter();

		/**
		 * @brief The cache type to format into a string
		 *
		 * @param 	type			The cache type to address
		 * @return	the string		The string formatted
		 */
		static msg_t* format(cache_t type);

	/*
	 * The Private Access methods
	 */
	private:

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
		static char* convert_time(uint32_t time);

};

/**< Typedef */
typedef formatter formatter_t;

#endif /* SERVICE_SERVICES_FORMATTER_FORMATTER_H_ */

/*
 * status_codes.h
 *
 *  Created on: Jun 9, 2015
 *      Author: francis-ccs
 */

#ifndef MODULES_COMMON_STATUS_CODES_H_
#define MODULES_COMMON_STATUS_CODES_H_

/**
 * \file
 *
 * \brief Status code definitions.
 *
 * This file defines various status codes returned by functions,
 * indicating success or failure as well as what kind of failure.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Francis Papineau microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * Status code that may be returned by shell commands and protocol
 * implementations.
 *
 * \note Any change to these status codes and the corresponding
 * message strings is strictly forbidden. New codes can be added,
 * however, but make sure that any message string tables are updated
 * at the same time.
 */
enum status_code {
	STATUS_OK               =  0, //!< Success
	STATUS_ERR_BUSY         =  0x19,
	STATUS_ERR_DENIED       =  0x1C,
	STATUS_ERR_TIMEOUT      =  0x12,
	ERR_IO_ERROR            =  -1, //!< I/O error
	ERR_FLUSHED             =  -2, //!< Request flushed from queue
	ERR_TIMEOUT             =  -3, //!< Operation timed out
	ERR_BAD_DATA            =  -4, //!< Data integrity check failed
	ERR_PROTOCOL            =  -5, //!< Protocol error
	ERR_UNSUPPORTED_DEV     =  -6, //!< Unsupported device
	ERR_NO_MEMORY           =  -7, //!< Insufficient memory
	ERR_INVALID_ARG         =  -8, //!< Invalid argument
	ERR_BAD_ADDRESS         =  -9, //!< Bad address
	ERR_BUSY                =  -10, //!< Resource is busy
	ERR_BAD_FORMAT          =  -11, //!< Data format not recognized
	ERR_NO_TIMER            =  -12, //!< No timer available
	ERR_TIMER_ALREADY_RUNNING   =  -13, //!< Timer already running
	ERR_TIMER_NOT_RUNNING   =  -14, //!< Timer not running
	ERR_ABORTED             =  -15, //!< Operation aborted by user
	/**
	 * \brief Operation in progress
	 *
	 * This status code is for driver-internal use when an operation
	 * is currently being performed.
	 *
	 * \note Drivers should never return this status code to any
	 * callers. It is strictly for internal use.
	 */
	OPERATION_IN_PROGRESS	= -128,
};

typedef enum status_code status_code_t;

#endif /* MODULES_COMMON_STATUS_CODES_H_ */

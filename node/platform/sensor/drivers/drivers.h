/*
 * drivers.h
 *
 *  Created on: Jun 11, 2015
 *      Author: francis-ccs
 */

#ifndef MODULES_DRIVERS_DRIVERS_H_
#define MODULES_DRIVERS_DRIVERS_H_

#include <platform/platform.h>

/*!
 * This includes the driver files based on the defined
 * sensor needed for the system daq task.
 */

#ifdef INCLUDE_TMP006
#include "ti/tmp006.h"
#endif

#ifdef INCLUDE_BMA222
#include "bosch/bma222.h"
#endif
#endif /* MODULES_DRIVERS_DRIVERS_H_ */

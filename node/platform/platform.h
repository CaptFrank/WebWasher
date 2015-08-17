/*
 * platform.h
 *
 *  Created on: Jul 4, 2015
 *      Author: francis-ccs
 */

#ifndef SENSORS_PLATFORM_H_
#define SENSORS_PLATFORM_H_

/*!
 * \brief Sensors that are to be included
 */
#define INCLUDE_BMA222
#define INCLUDE_TMP006

/*
 * Platform includes:
 * 	- I2c bus
 * 	- Sensors (tmp006, bma222)
 * 	- Wifi
 * 	- Mqtt
 */
#include "bus/i2c/bus_i2c.h"
#include "drivers/drivers.h"
#include "iface/wifi.h"
#include "iface/mqtt.h"

/*
 * Include the queue
 */
#include "others/queue.h"

/*
 * Include the caches
 */
#include "others/caches.h"

#endif /* SENSORS_PLATFORM_H_ */

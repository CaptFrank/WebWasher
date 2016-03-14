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

// #define BMA222_IRQ // Attach interrupt

/*
 * Platform includes:
 * 	- I2c bus
 * 	- Sensors (tmp006, bma222)
 * 	- Mqtt
 */
#include "iface/mqtt.h"
#include "bus/i2c/bus_i2c.h"
#include "sensor/drivers/drivers.h"

/*
 * Include the queue
 */
#include "others/queue.h"

/*
 * Include the caches
 */
#include "others/caches.h"

#endif /* SENSORS_PLATFORM_H_ */

/*
 * sensori2c.cpp
 *
 *  Created on: Jul 6, 2015
 *      Author: francis-ccs
 */

#include <platform/sensor/sensor/i2c/sensor_i2c.h>

/*!
 * \brief Initialize the bus I/O interface.
 *
 * @param resolution		The resolution of th transaction
 * @param bus				The sensor bus
 */
sensor_i2c::sensor_i2c(bus_i2c_t* iface, uint8_t resolution): sensor() {

	/*
	 * Set internals
	 */

	bus 		= iface;
	set_resolution(resolution);
}

/**
 * \brief Sets the transaction resolution
 *
 * @param resolution		The resolution of th transaction
 */
void sensor_i2c::set_resolution(uint8_t resolution){

	/*
	 * Set the resolution internally
	 */
	transaction_resolution = resolution;
}

/**
 * @brief Gets the transaction resolution
 *
 * @return resolution		The resolution that was set
 */
uint8_t sensor_i2c::get_resolution(){

	/*
	 * Return the resolution
	 */
	return transaction_resolution;
}

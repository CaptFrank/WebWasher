/*
 * sensori2c.h
 *
 *  Created on: Jul 6, 2015
 *      Author: francis-ccs
 */

#ifndef PLATFORM_SENSOR_SENSOR_I2C_SENSOR_I2C_H_
#define PLATFORM_SENSOR_SENSOR_I2C_SENSOR_I2C_H_

#include <platform/bus/i2c/bus_i2c.h>
#include <platform/sensor/sensor/sensor.h>

/**
 * @brief The I2C Sensor Device Interface
 *
 * This is the class that combines both the sensor type object with
 * the i2c bus device driver. We need this class to combine both
 * the methods that are within each contexts.
 *
 * @extends senor_t
 * @extends bus_i2c_t
 */
class sensor_i2c : public sensor {

	/*
	 * Private context
	 */
	private:

		bus_packet_t				packet;						/**< Internal bus packet */
		uint8_t						transaction_resolution;		/**< Transaction Resolution (16bit/8bit) */

	/*
	 * Protected class methods
	 */
	protected:

		/*
		 * Bus Handle
		 */
		bus_i2c_t*					bus;						/**< Bus Handle */
		cache_t						cache_type;					/**< Sensor cache type */

		/*!
		 * \brief Initialize the bus I/O interface.
		 *
		 * @param resolution		The resolution of th transaction
		 * @param iface				The sensor bus
		 */
		sensor_i2c(bus_i2c_t* iface, uint8_t resolution);

		/*!
		 * \brief The Default Deconstructor for the Object
		 */
		~sensor_i2c(){}

		/**
		 * \brief Sets the transaction resolution
		 *
		 * @param resolution		The resolution of th transaction
		 */
		void set_resolution(uint8_t resolution);

		/**
		 * @brief Gets the transaction resolution
		 *
		 * @return resolution		The resolution that was set
		 */
		uint8_t get_resolution();
};

/**
 * @brief Typedef
 */
typedef sensor_i2c sensor_i2c_t;
#endif /* PLATFORM_SENSOR_SENSOR_I2C_SENSOR_I2C_H_ */

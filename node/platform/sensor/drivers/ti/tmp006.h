/*
 * tmp006.h
 *
 *  Created on: Jul 7, 2015
 *      Author: fpapinea
 */

#ifndef PLATFORM_SENSOR_DRIVERS_TI_TMP006_H_
#define PLATFORM_SENSOR_DRIVERS_TI_TMP006_H_

#include <platform/sensor/sensor/i2c/sensor_i2c.h>

/* TWI/I2C address (write @ 0x16 on bus, read @ 0x17 on bus) */
#define TMP006_I2C_ADDR         	(0x18)

/* Sensor Data Resolution and Offsets */
#define TMP006_DATA_RESOLUTION  	(16)    	/* signed axis data size (bits) */
#define TMP006_TRANSACTION_BYTE		(TMP006_DATA_RESOLUTION / 8)
#define TMP006_TEMP_OFFSET      	(0)     	/* temperature center (Celsius) */

#define TMP006_CELCIUS_CONV			(0.03125)
#define TMP006_KELVIN_CONV			(273.15)

#define TMP006_B0 					(-0.0000294)
#define TMP006_B1 					(-0.00000057)
#define TMP006_B2 					(0.00000000463)
#define TMP006_C2 					(13.4)
#define TMP006_TREF 				(298.15)
#define TMP006_A2 					(-0.00001678)
#define TMP006_A1 					(0.00175)
#define TMP006_S0 					(6.4)  // * 10^-14

/*
 * Standard Register Addresses (TWI & SPI)
 *
 * w/r=write/read, wo=write only, ro=read only
 */
typedef enum {

	TMP006_BURST_ADDR				= 0x00,
	TMP006_VOLTAGE					= 0x00,		/* 00 (ro) sensor voltage */
	TMP006_TEMPERATURE,							/* 01 (r0) sensor temperature */
	TMP006_CONFIGURATION,						/* 02 (w/r) sensor configuration */

	//...
	TMP006_ID_ADDR					= 0xFE,
	TMP006_MAN_ID					= 0xFE,		/* 03 (ro) sensor manufacture id */
	TMP006_CHIP_ID,								/* 04 (ro) sensor chip id */
} tmp006_register_t;

/*
 * Conversion rates for the temperature sensor
 */
typedef enum {

	TMP006_CONV_RATE_4Hz		= 0x0000,
	TMP006_CONV_RATE_2Hz		= 0x0200,
	TMP006_CONV_RATE_1Hz		= 0x0400,
	TMP006_CONV_RATE_05Hz		= 0x0600,
	TMP006_CONV_RATE_025Hz		= 0x0800

} tmp006_conv_rate_t;

/*
 * Self test interface
 */
typedef struct {

	int test_code;
	void* arg;
} tmp006_self_test_t;

/** \brief Sensor Event Registers */
typedef struct {
	uint16_t volt;                 			/**< Voltage data */
	uint16_t temp;                       	/**< Temperature data */

	union {
		uint16_t status_byte;       		/**< Status bytes */
		struct {                       		/**< Status fields */

			uint8_t soft_reset		: 1;	/**< Software reset */
			uint8_t modes			: 3;	/**< Software modes */
			uint8_t conversion		: 3;	/**< Conversion rate */
			uint8_t enable			: 1;	/**< Enable flag */
			uint8_t data_rdy		: 1;	/**< Data ready flag */
			uint8_t unused			: 7;	/**< Reserved */
		} status_field;
	};
} tmp006_event_regs_t;

/** \brief Sensor id regs **/
typedef struct {

	uint16_t man_id;						/**< Manufacturer id */
	uint16_t dev_id;						/**< Device id */

}tmp006_id_regs_t;

/** @brief The double temperatures */
typedef struct {
	double die_temp;
	double obj_temp;
}tmp006_temps_t;

/** \brief TMP006 Register Bit Definitions */
/** @{ */

/* TMP006 Sensor Configuration 		(0x02) */

//! Software reset bit configuration
#define TMP006_SOFT_RESET			(1 << 15)

//! Mode settings
#define TMP006_POWER_DOWN			(0x0000)
#define TMP006_CONT_CONV			(0x7000)

//! Enable bit settings
#define TMP006_ENABLE				(1 << 9)
#define TMP006_DISABLE				(0 << 0)

//! Data ready bit settings
#define TMP006_DATA_READY			(1 << 8)
#define TMP006_CONV_PROG			(0 << 8)

/* TMP006 Manufacturer Id 			(0xFE)*/

#define TMP006_MANUFACTURER_ID		(0x5449)

/* TMP006 Chip Id 					(0xFF)*/

#define TMP006_ID_VAL				(0x0067)

/** @brief Data definition */
typedef sensor_data_t tmp006_data_t;

/**
 * @brief TMP006 Cache
 */
typedef struct {

	tmp006_data_t					temp_die;
	tmp006_data_t					temp_obj;
	tmp006_data_t					voltage;
	tmp006_temps_t					temps;
}tmp006_cache_t;

/**
 * \brief The TMP006 I2C Bus Interface
 *
 * 	This it the bus driver interface class that incorporates all necessary
 * 	bus structures and needed definitions.
 *
 * 	Note: This sensor type does not support events. The sensor must
 * 			be polled constently to get data.
 *
 * 	@extends sensor_i2c_t
 */
class tmp006: public sensor_i2c_t {

	/*
	 * Private class attributes
	 */
	private:

		/*
		 * Attributes
		 */
		tmp006_event_regs_t				regs;
		tmp006_id_regs_t				id;

		/*
		 * Data
		 */
		tmp006_cache_t 					cache;

	/*
	 * Public class methods
	 */
	public:

		/*!
		 *\brief The default constructor for the class.
		 *
		 * Nothing is done in the constructor as this class is mearly an
		 * interface to the sensor class.
		 */
		tmp006(bus_i2c_t* iface);

		/*!
		 *\brief The default deconstructor for the class.
		 *
		 * Nothing is done in the constructor as this class is mearly an
		 * interface to the sensor class.
		 */
		~tmp006(){};

		/**
		 * \brief Updates the internal cache of the sensor.
		 *
		 * In this function, the system reads the newest data from the sensor and
		 * updates the internal caches that are mapped to the internal system BIOS.
		 * We do this to obtain a transparency between the BIOS and the sensor.
		 *
		 * @return	bool 	true of the update was successful.
		 */
		bool run();

		/**
		 * \brief Read sensor data
		 *
		 *  This is a generic routine that will call a specified sensor function
		 *  returning \c vector sensor data.  It is used primarily by API routines
		 *  but may be called by special-case application or library code where
		 *  needed to extend the API semantics.
		 *
		 *  As an example, consider a multiple-axis gyroscope driver implementing
		 *  the sensor_funcs_t.get_rotation() interface.  An application will usually
		 *  call the sensor_get_rotation() routine in this case, but the angular
		 *  rate might be obtained from the device as follows:
		 *
		 *  \code

			  sensor_t      gyroscope;
			  sensor_data_t omega;
			  ...

			  sensor_read(&gyroscope, SENSOR_READ_ROTATION, &omega);
			  ...

		\endcode
		 *
		 * In the event of a false return from this routine, the contents stored
		 * at the location specified by "data" are undefined.
		 *
		 * \param   type    Type of read operation to perform.
		 * \return  bool    true if the call succeeds, else false is returned.
		 */
		bool read(sensor_read_t type);

		/**
		 * \brief Calibrate a sensor device.
		 *
		 * \param   caltype The type of calibration to perform.
		 * \param   code    Device-specific calibration code or step parameter.
		 * \param   info    Unimplemented (ignored) parameter.
		 * \return  bool    true if the call succeeds, else false is returned.
		 */
		bool calibrate(sensor_calibration_t caltype, int code, void *info);

		/**
		 * \brief Initiate a sensor device software reset.
		 *
		 * \param   arg     Device-specific argument options.
		 * \return  bool    true if the call succeeds, else false is returned.
		 */
		bool reset(int arg);

		/**
		 * \brief Set a sensor device to low-power or standby mode.
		 *
		 * \param   arg     Device-specific argument options.
		 * \return  bool    true if the call succeeds, else false is returned.
		 */
		bool sleep(int arg);

		/**
		 * \brief Execute a sensor device control function.
		 *
		 * \param   cmd     Specifies the IOCTL command.
		 * \param   arg     Specifies command parameters (varies by command).
		 * \return  bool    true if the call succeeds, else false is returned.
		 */
		bool ioctl(sensor_command_t cmd, void *arg);

		/**
		 * \brief Activate a sensor self-test function.
		 *
		 * \param test_code Address of a device-specific numeric test code.
		 * \param arg       Device-specific self-test argument options.
		 * \return bool     true if the test succeeds, else false is returned.
		 */
		bool selftest(int *test_code, void *arg);

		/**
		 * \brief Set a sensor mode.
		 *
		 * This routine sets a specified sensor execution state to one of the
		 * following values:
		 *
		 *      - SENSOR_STATE_NORMAL
		 *      - SENSOR_STATE_SLEEP
		 *      - SENSOR_STATE_SUSPEND
		 *      - SENSOR_STATE_LOWEST_POWER
		 *      - SENSOR_STATE_LOW_POWER
		 *      - SENSOR_STATE_HIGH_POWER
		 *      - SENSOR_STATE_HIGHEST_POWER
		 *      - SENSOR_STATE_RESET
		 *
		 * These execution states are not supported in all devices.  The function
		 * return value will indicate whether or not the request could be processed.
		 * The result of the request, when supported, is device dependent.
		 *
		 * \param   mode    A specified sensor operational mode.
		 * \return  bool    true if the call succeeds, else false is returned.
		 */
		bool set_state(sensor_state_t mode);

		/**
		 * \brief Get a sensor mode.
		 *
		 * This routine gets the current sensor execution state which may be one
		 * of the following values:
		 *
		 *      - SENSOR_STATE_NORMAL
		 *      - SENSOR_STATE_SLEEP
		 *      - SENSOR_STATE_SUSPEND
		 *      - SENSOR_STATE_LOWEST_POWER
		 *      - SENSOR_STATE_LOW_POWER
		 *      - SENSOR_STATE_HIGH_POWER
		 *      - SENSOR_STATE_HIGHEST_POWER
		 *      - SENSOR_STATE_RESET
		 *
		 * These execution states are not supported in all devices.  The function
		 * return value will indicate whether or not the request could be processed.
		 * The result of the request, when supported, is device dependent.
		 *
		 * \param   mode    The current sensor mode is returned to this location.
		 * \return  bool    true if the call succeeds, else false is returned.
		 */
		bool get_state(sensor_state_t *mode);

	/*
	 * Private class methods
	 */
	private:

		/**
		 * \brief Get sensor hardware device ID.
		 *
		 * This routine returns device-specific sensor hardware identification
		 * and, optionally, version values.  Unimplemented values will be set
		 * to zero.  For example, devices supporting a dedicated ID register, but
		 * not a version register, will set "id" to the ID value and "ver" to zero.
		 *
		 * \return  bool    true if the call succeeds, else false is returned.
		 */
		bool get_device_id();

		/**
		 * @brief Read TMP006 temperature data.
		 *
		 * This function obtains temperature data from the Texas Instruments
		 * device.  The data is read from the 1 16 bit register.
		 *
		 * Along with the actual sensor data, the LSB byte contains a "new" flag
		 * indicating if the data for this axis has been updated since the last
		 * time the axis data was read.  Reading either LSB or MSB data will
		 * clear this flag.
		 *
		 * @return bool     true if the call succeeds, else false is returned.
		 */
		bool get_obj_temp();

		/**
		 * @brief Read TMP006 temperature data.
		 *
		 * This function obtains temperature data from the Texas Instruments
		 * device.  The data is read from the 1 16 bit register.
		 *
		 * Along with the actual sensor data, the LSB byte contains a "new" flag
		 * indicating if the data for this axis has been updated since the last
		 * time the axis data was read.  Reading either LSB or MSB data will
		 * clear this flag.
		 *
		 * @return bool     true if the call succeeds, else false is returned.
		 */
		bool get_die_temp();

		/**
		 * @brief Read TMP006 voltage data.
		 *
		 * This function reads voltage data
		 *
		 * @return bool     true if the call succeeds, else false is returned.
		 */
		bool get_volt();

		/**
		 * @brief Enable or disable the TMP006 sensor.
		 *
		 * This routine enables or disables the TMP006 depending upon
		 * the value of the \sleep parameter; a \true value enables the sensor and
		 * a \false value disables the sensor by setting or clearing the 'en'
		 * bit, respectively.
		 *
		 * @param enable     Set flag \true to enable sleep mode.
		 * @return Nothing
		 */
		void enable(bool enable);

		/**
		 * @brief Power On or Power off the TMP006 sensor.
		 *
		 * This routine powers on or powers off the TMP006 depending upon
		 * the value of the \sleep parameter; a \true value enables the sensor and
		 * a \false value disables the sensor by setting or clearing the 'en'
		 * bit, respectively.
		 *
		 * @param enable     Set flag \true to enable sleep mode.
		 * @return Nothing
		 */
		void poweroff(bool enable);

		/**
		 * @brief Set conversion timing value
		 *
		 * @param rate      Set the rate of conversion.
		 * @return bool     true if the call succeeds, else false is returned.
		 */
		bool set_conv_rate (tmp006_conv_rate_t *rate);

		/**
		 * @brief Get conversion timing value
		 *
		 * @param rate      Set the rate of conversion.
		 * @return bool     true if the call succeeds, else false is returned.
		 */
		bool get_conv_rate (tmp006_conv_rate_t *rate);

		/**
		 * @brief Write to the config regs.
		 *
		 * @param value		The value to write
		 */
		bool edit_conf(uint16_t value);

		/**
		 * @brief Get event threshold value
		 *
		 * @param threshold Address of threshold descriptor.
		 * @return bool     true if the call succeeds, else false is returned.
		 */
		bool get_threshold(sensor_threshold_desc_t *threshold);

		/**
		 * @brief Set event threshold value
		 *
		 * @param threshold Address of threshold descriptor.
		 * @return bool     true if the call succeeds, else false is returned.
		 */
		bool set_threshold(sensor_threshold_desc_t *threshold);

};

/**< @brief Typedef */
typedef tmp006 tmp006_t;

#endif /* PLATFORM_SENSOR_DRIVERS_TI_TMP006_H_ */

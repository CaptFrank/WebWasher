/*
 * tmp006.cpp
 *
 *  Created on: Jul 7, 2015
 *      Author: fpapinea
 */

#include <platform/sensor/drivers/ti/tmp006.h>

/** \brief TI TMP006 Range Table (NULL)
 *
 * This sensor does not have a selective range descriptor.
 * It is a fixed range device. Thus the implementation of the
 * range table is declared as a NULL table.
 */
static const sensor_map_t range_table [] = {NULL};

/** \brief TI TMP006 Bandwidth Table (NULL)
 *
 * This sensor does not have a selective bandwidth descriptor.
 * It is a fixed bandwidth device. Thus the implementation of the
 * bandwidth table is declared as a NULL table.
 */
static const sensor_map_t band_table [] = {NULL};

/*!
 *\brief The default constructor for the class.
 *
 * Nothing is done in the constructor as this class is mearly an
 * interface to the sensor class.
 *
 */
tmp006::tmp006(bus_i2c_t* iface) : sensor_i2c(iface, TMP006_TRANSACTION_BYTE){

	/*
	 * Get the device id
	 */
	if (!get_device_id()){

		/*
		 * IO issue
		 */
		err = SENSOR_ERR_DRIVER;
		return;
	}

	/*
	 * Check for validity
	 */
	if (TMP006_ID_VAL == id.dev_id) {

		/*
		 * Set the driver function table and capabilities pointer.
		 */
		caps.feature			= 	(sensor_feature_t) 	(SENSOR_CAPS_SELFTEST   |
														SENSOR_CAPS_TEMPERATURE |
														SENSOR_CAPS_VOLTAGE);

		caps.vendor				= 	SENSOR_VENDOR_TI;
		caps.range_table		= 	range_table;
		caps.range_count		= 	ARRAYSIZE(range_table);
		caps.band_table			= 	band_table;
		caps.band_count			= 	ARRAYSIZE(band_table);
		caps.units				= 	SENSOR_UNITS_deg_Celcius;
		caps.scale				= 	SENSOR_SCALE_one;
		caps.name				= 	"TMP006 Digital temperature sensor";

		/*
		 * Set the driver (device) default configurations and
		 * reset its state machine.
		 */
		set_state(SENSOR_STATE_RESET);

		/*
		 * Set the hal object.
		 */
		hal.range      			= 200;
		hal.bandwidth  			= NULL;
		hal.resolution 			= TMP006_DATA_RESOLUTION;
		hal.burst_addr 			= TMP006_BURST_ADDR;

		/*
		 * Set Sensor Type
		 */
		type					= (sensor_type_t)(SENSOR_TYPE_TEMPERATURE |
								   	   	   	   	   SENSOR_TYPE_VOLTAGE);

		/*
		 * Set the message type
		 */
		msg 					= MSG_TYPE_TEMP_DATA;

		/*
		 * Set cache type
		 */
		cache_type				= CACHE_TYPE_TEMP_DATA;

		/* Check bus status and return true if ok */
		if (bus->get_status() == STATUS_OK){

			/*
			 * No Errors
			 */
			err = SENSOR_ERR_NONE;
			return;

		}else{

			/*
			 * Error present
			 */
			err = SENSOR_ERR_DRIVER;
			return;

		}
	}
}

/**
 * \brief Updates the internal cache of the sensor.
 *
 * In this function, the system reads the newest data from the sensor and
 * updates the internal caches that are mapped to the internal system BIOS.
 * We do this to obtain a transparency between the BIOS and the sensor.
 *
 * @return	bool 	true of the update was successful.
 */
bool tmp006::update(){

	/*
	 * Return code
	 */
	bool rc;

	/*
	 * Turn on the device
	 */
	rc = set_state(SENSOR_STATE_NORMAL);

	/*
	 * We read all the pertinent values from the sensor.
	 */
	rc |= read(SENSOR_READ_OBJ_TEMPERATURE);
	rc |= read(SENSOR_READ_DIE_TEMPERATURE);
	rc |= read(SENSOR_READ_VOLTAGE);

	/*
	 * Sleep the device
	 */
	rc |= set_state(SENSOR_STATE_SLEEP);

	return (rc);
}


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
bool tmp006::read(sensor_read_t type){

	/*
	 * Switch on the sensor vector to read
	 */
	switch (type) {

	/*
	 * Read the object temperature
	 */
	case SENSOR_READ_OBJ_TEMPERATURE:
		return get_obj_temp();

	/*
	 * Read the die temperature
	 */
	case SENSOR_READ_DIE_TEMPERATURE:
		return get_die_temp();

	/*
	 * Read the voltage
	 */
	case SENSOR_READ_VOLTAGE:
		return get_volt();

	/*
	 * Read the device id
	 */
	case SENSOR_READ_ID:
		return get_device_id();

	/*
	 * Not supported
	 */
	default:
		err = SENSOR_ERR_FUNCTION;
		return false;
	}

}

/**
 * \brief Calibrate a sensor device.
 *
 * \param   caltype The type of calibration to perform.
 * \param   code    Device-specific calibration code or step parameter.
 * \param   info    Unimplemented (ignored) parameter.
 * \return  bool    true if the call succeeds, else false is returned.
 */
bool tmp006::calibrate(sensor_calibration_t caltype, int code, void *info){

	/*
	 * This calibration routine is not supported in the
	 * TMP006 sensor.
	 */
	err = SENSOR_ERR_UNSUPPORTED;
	return true;
}

/**
 * \brief Initiate a sensor device software reset.
 *
 * \param   arg     Device-specific argument options.
 * \return  bool    true if the call succeeds, else false is returned.
 */
bool tmp006::reset(int arg){

	/*
	 * Create the command
	 */
	regs.status_byte ^= TMP006_SOFT_RESET;

	/*
	 * Write the command twice for reset
	 */
	if(!edit_conf(regs.status_byte)){

		/*
		 * IO Error
		 */
		err = SENSOR_ERR_IO;
		return false;
	}
	if(!edit_conf(regs.status_byte)){

		/*
		 * IO Error
		 */
		err = SENSOR_ERR_IO;
		return false;
	}

	/*
	 * No problem
	 */
	return true;
}

/**
 * \brief Set a sensor device to low-power or standby mode.
 *
 * \param   arg     Device-specific argument options.
 * \return  bool    true if the call succeeds, else false is returned.
 */
bool tmp006::sleep(int arg){

	/*
	 * Power off the device
	 */
	poweroff(true);
	return true;
}

/**
 * \brief Set a sensor operational threshold.
 *
 * \param   threshold   A specified sensor operational threshold.
 * \param   value       The value of the specified threshold.
 * \return  bool     true if the call succeeds, else false is returned.
 */
bool tmp006::set_threshold(sensor_threshold_t threshold, int16_t value){

	/*
	 * This threshold routine is not supported in the
	 * TMP006 sensor.
	 */
	err = SENSOR_ERR_UNSUPPORTED;
	return true;
}

/**
 * \brief Get a sensor operational threshold.
 *
 * \param   sensor      The address of an initialized sensor descriptor.
 * \param   threshold   A specified sensor operational threshold.
 * \param   value       Address of location to return threshold value
 *
 * \return  bool     true if the call succeeds, else false is returned.
 */
bool tmp006::get_threshold(sensor_threshold_t threshold, int16_t *value){

	/*
	 * This threshold routine is not supported in the
	 * TMP006 sensor.
	 */
	err = SENSOR_ERR_UNSUPPORTED;
	return true;
}

/**
 * \brief Execute a sensor device control function.
 *
 * \param   cmd     Specifies the IOCTL command.
 * \param   arg     Specifies command parameters (varies by command).
 * \return  bool    true if the call succeeds, else false is returned.
 */
bool tmp006::ioctl(sensor_command_t cmd, void *arg){

	// Container
	sensor_state_t mode;

	/*
	 * Switch on the commands
	 */
	switch (cmd) {

	/*
	 * Unsupported
	 */
	/*
	 * Events are not supported
	 */
	case SENSOR_ENABLE_EVENT:
	case SENSOR_DISABLE_EVENT:

	default:
		err = SENSOR_ERR_UNSUPPORTED;
		return false;

	/*
	 * Setting the state
	 */
	case SENSOR_SET_STATE:
	{
		mode = *((sensor_state_t *)arg);

		if ((mode != mod) && set_state(mode)) {
			mod = (mode == SENSOR_STATE_RESET)
					? SENSOR_STATE_NORMAL : mode;
			return true;
		} else {
			return false;
		}
	};

	case SENSOR_SET_SAMPLE_RATE:
	{
		return set_conv_rate((tmp006_conv_rate_t *)arg);
	};

	case SENSOR_GET_SAMPLE_RATE:
	{
		return get_conv_rate((tmp006_conv_rate_t *)arg);
	};

	case SENSOR_READ_SCALAR:
	{
		if(! get_die_temp()){

			/*
			 * Problem
			 */
			return false;
		}else if(! get_obj_temp()){

			/*
			 * Problem
			 */
			return false;
		}else if(! get_volt()){

			/*
			 * Problem
			 */
			return false;
		}else {

			/*
			 * All good
			 */
			return true;
		}
	};
	}
}

/**
 * \brief Activate a sensor self-test function.
 *
 * \param test_code Address of a device-specific numeric test code.
 * \param arg       Device-specific self-test argument options.
 * \return bool     true if the test succeeds, else false is returned.
 */
bool tmp006::selftest(int *test_code, void *arg){

	/*
	 * This selftest routine is not supported in the
	 * TMP006 sensor.
	 */
	err = SENSOR_ERR_UNSUPPORTED;
	return true;
}

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
bool tmp006::set_state(sensor_state_t mode){

		/* Perform a state transition out of the previous mode
		 * into the new mode.
		 */

		switch (mod) {
		case SENSOR_STATE_NORMAL:
		case SENSOR_STATE_HIGHEST_POWER:

			if((SENSOR_STATE_POWER_DOWN == mode) ||
					(SENSOR_STATE_LOW_POWER == mode) ||
					(SENSOR_STATE_LOWEST_POWER == mode)){

				// We power it on and disable it
				poweroff(false);

				// Change mode to active
				mode = SENSOR_STATE_NORMAL;
			}

			// Sensor on ??
			if((SENSOR_STATE_SLEEP == mode) ||
					SENSOR_STATE_SUSPEND == mode){

				// We disable the device
				enable(true);
			}

			break;

		case SENSOR_STATE_SLEEP:
		case SENSOR_STATE_SUSPEND:

			if((SENSOR_STATE_POWER_DOWN == mode) ||
					(SENSOR_STATE_LOW_POWER == mode) ||
					(SENSOR_STATE_LOWEST_POWER == mode)){

				// We power it on and disable it
				poweroff(false);

				// Change mode to active
				mode = SENSOR_STATE_NORMAL;
			}

			// Sensor on ??
			if((SENSOR_STATE_NORMAL == mode) ||
					SENSOR_STATE_HIGHEST_POWER == mode){

				// We disable the device
				enable(false);
			}
			break;

		case SENSOR_STATE_POWER_DOWN:
		case SENSOR_STATE_LOW_POWER:
		case SENSOR_STATE_LOWEST_POWER:

			// Power down the device
			poweroff(false);
			break;

		case SENSOR_STATE_RESET:

			// Reset the device
			reset(0);
			break;

		default:
			return false;
		}
		return true;
}

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
bool tmp006::get_state(sensor_state_t *mode){

	/*
	 * Get the stored device state
	 */
	*mode = mod;
	return true;
}

/*
* Private class methods
*/

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
bool tmp006::get_device_id(){

	/*
	 * Get the device id
	 */
	if(bus->read(
				TMP006_I2C_ADDR,				// Destination
				sizeof(tmp006_id_regs_t),		// Size to read
				(uint8_t)TMP006_ID_ADDR,		// Memory index to read from
				(uint8_t*)&id					// Where to store the value
			) != sizeof(tmp006_id_regs_t)){

		/*
		 * Error present
		 */
		err = SENSOR_ERR_DRIVER;
		return false;
	}

	/*
	 * Return the bus status
	 */
	return (STATUS_OK == bus->get_status());
}

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
bool tmp006::get_obj_temp(){

	/*
	 * We read the object temperature
	 */
	get_volt();
	get_die_temp();

	/*
	 * Get the values
	 */
	double die_temp = cache.temp_die.temperature.value;
	double obj_volt = cache.voltage.voltage.value;

	obj_volt 			*= 156.25;  // 156.25 nV per LSB
	obj_volt 			/= 1000; // nV -> uV
	obj_volt 			/= 1000; // uV -> mV
	obj_volt 			/= 1000; // mV -> V
	die_temp 			*= TMP006_CELCIUS_CONV; // convert to celsius
	die_temp 			+= TMP006_KELVIN_CONV; // convert to kelvin

	double tdie_tref 	= die_temp - TMP006_TREF;

	double S 			= (1 + TMP006_A1 * tdie_tref +
					 	 	 TMP006_A2 * tdie_tref * tdie_tref);

	S 					*= TMP006_S0;
	S 					/= 10000000;
	S 					/= 10000000;

	double s_volt 		= TMP006_B0 + TMP006_B1 * tdie_tref +
							TMP006_B2 * tdie_tref * tdie_tref;

	double f_obj_volt 	= (obj_volt - s_volt) + \
			TMP006_C2 * (obj_volt - s_volt) * (obj_volt - s_volt);

	double obj_temp = sqrt(sqrt(die_temp * die_temp * die_temp * die_temp + f_obj_volt / S));

	obj_temp -= TMP006_KELVIN_CONV; // Kelvin -> *C

	/*
	 * Save the value
	 */
	cache.temp_obj.temperature.value = obj_temp;
	cache.temps.obj_temp = obj_temp;

	return true;
}

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
bool tmp006::get_die_temp(){

	if(TMP006_TRANSACTION_BYTE != bus->read(
				TMP006_I2C_ADDR,				// Destination
				TMP006_TRANSACTION_BYTE,		// Size to read
				(uint8_t)TMP006_TEMPERATURE,	// Memory index to read from
				(uint8_t*)&cache.temp_die.temperature.value	// Where to store the value
			)){

		/*
		 * Error present
		 */
		err = SENSOR_ERR_DRIVER;
		return false;
	}


	/*
	 * Set the double value
	 */
	cache.temps.die_temp = \
			cache.temp_die.temperature.value * TMP006_CELCIUS_CONV;

	/*
	 * Return the bus status
	 */
	return (STATUS_OK == bus->get_status());

}

/**
 * @brief Read TMP006 voltage data.
 *
 * This function reads voltage data
 *
 * @return bool     true if the call succeeds, else false is returned.
 */
bool tmp006::get_volt(){

	if(TMP006_TRANSACTION_BYTE != bus->read(
				TMP006_I2C_ADDR,				// Destination
				TMP006_TRANSACTION_BYTE,		// Size to read
				(uint8_t)TMP006_VOLTAGE,		// Memory index to read from
				(uint8_t*)&cache.voltage.voltage.value	// Where to store the value
			)){

		/*
		 * Error present
		 */
		err = SENSOR_ERR_DRIVER;
		return false;
	}

	/*
	 * Return the bus status
	 */
	return (STATUS_OK == bus->get_status());

}

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
void tmp006::enable(bool enable){

	uint16_t const power_mode_val = (enable == true)
			? (TMP006_ENABLE) : (TMP006_DISABLE);

	// Container
	regs.status_byte |= power_mode_val;

	/*
	 * Return the state of the bus
	 */
	edit_conf(regs.status_byte);
	return;
}

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
void tmp006::poweroff(bool enable){

	// Container
	uint16_t const power_mode_val = (enable == true)
			? (TMP006_POWER_DOWN) : (TMP006_CONT_CONV);


	// Container
	regs.status_byte |= power_mode_val;

	/*
	 * Return the state of the bus
	 */
	edit_conf(regs.status_byte);
	return;

}

/**
 * @brief Set conversion timing value
 *
 * @param rate      Set the rate of conversion.
 * @return bool     true if the call succeeds, else false is returned.
 */
bool tmp006::set_conv_rate (tmp006_conv_rate_t *rate){

	// Container
	regs.status_byte |= (uint16_t)(*rate);

	/*
	 * Return the state of the bus
	 */
	return edit_conf(regs.status_byte);
}

/**
 * @brief Get conversion timing value
 *
 * @param rate      Set the rate of conversion.
 * @return bool     true if the call succeeds, else false is returned.
 */
bool tmp006::get_conv_rate (tmp006_conv_rate_t *rate){

	if(sizeof(tmp006_event_regs_t) != bus->read(
				TMP006_I2C_ADDR,					// Destination
				sizeof(tmp006_event_regs_t),		// Size to read
				(uint8_t)TMP006_BURST_ADDR,			// Memory index to read from
				(uint8_t*)&regs						// Where to store the value
			)){

		/*
		 * Error present
		 */
		err = SENSOR_ERR_DRIVER;
		return false;
	}

	/*
	 * Return the bus status
	 */
	*rate = (tmp006_conv_rate_t)regs.status_field.conversion;
	return (STATUS_OK == bus->get_status());
}

/**
 * @brief Write to the config regs.
 *
 * @param value		The value to write
 */
bool tmp006::edit_conf(uint16_t value){

	if(TMP006_TRANSACTION_BYTE != bus->read(
				TMP006_I2C_ADDR,					// Destination
				TMP006_TRANSACTION_BYTE,			// Size to read
				(uint8_t)TMP006_CONFIGURATION,		// Memory index to read from
				(uint8_t*)&value					// Where to store the value
			)){

		/*
		 * Error present
		 */
		err = SENSOR_ERR_DRIVER;
		return false;
	}

	/*
	 * Return the state of the bus
	 */
	return (STATUS_OK == bus->get_status());
}

/*
 * bma222.cpp
 *
 *  Created on: Jul 7, 2015
 *      Author: fpapinea
 */

#include <platform/sensor/drivers/bosch/bma222.h>

/*!
 *\brief The default constructor for the class.
 *
 * Nothing is done in the constructor as this class is mearly an
 * interface to the sensor class.
 */
bma222::bma222(bus_i2c_t* iface) : sensor_i2c(iface, BMA222_TRANSACTION_BYTE) {

	// Set the callbacks
	for(int i = 0; i < BMA222_CALLBACKS; i++){
		bma222::callbacks[i].handler = default_event_handler;
	}

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
	if (BMA222_ID_VAL == id.dev_id) {

		/*
		 * Set the driver function table and capabilities pointer.
		 */
		caps.feature			= 	(sensor_feature_t) 	(SENSOR_CAPS_3_AXIS     |
														SENSOR_CAPS_SELFTEST   |
														SENSOR_CAPS_HI_G_EVENT |
														SENSOR_CAPS_LO_G_EVENT |
														SENSOR_CAPS_TAP_EVENT  |
														SENSOR_CAPS_TILT_EVENT |
														SENSOR_CAPS_AUX_TEMP);

		caps.vendor				= 	SENSOR_VENDOR_BOSCH;
		caps.range_table		= 	range_table;
		caps.range_count		= 	ARRAYSIZE(range_table);
		caps.band_table			= 	band_table;
		caps.band_count			= 	ARRAYSIZE(band_table);
		caps.units				= 	SENSOR_UNITS_deg_Celcius;
		caps.scale				= 	SENSOR_SCALE_one;
		caps.name				= 	"BMA222 Digital, triaxial acceleration sensor";

		/*
		 * Set the driver (device) default configurations and
		 * reset its state machine.
		 */
		set_state(SENSOR_STATE_RESET);

		/*
		 * Set the hal object.
		 */
		hal.range      			= 2000;
		hal.bandwidth  			= 1000;
		hal.resolution 			= BMA222_DATA_RESOLUTION;
		hal.burst_addr 			= BMA222_NEW_DATA_X;

		/*
		 * Set Sensor Type
		 */
		type					= (sensor_type_t) (SENSOR_TYPE_ACCELEROMETER |
								   	   	   	   	   SENSOR_TYPE_TEMPERATURE);

		/*
		 * Set the message type
		 */
		msg 					= MSG_TYPE_ACC_DATA;

		/*
		 * Set cache type
		 */
		cache_type				= CACHE_TYPE_ACC_DATA;

		/* Check bus status and return true if ok */
		if ((STATUS_OK == bus->get_status())
#ifdef BMA222_IRQ
				&& irq_connect(BMA222_INT_PIN, CHANGE, bma222_t::isr)
#endif
		){ // Register the isr
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
bool bma222::update(){

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
	rc |= read(SENSOR_READ_ACCELERATION);
	rc |= read(SENSOR_READ_TEMPERATURE);

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
bool bma222::read(sensor_read_t type){

	switch (type) {
	case SENSOR_READ_ACCELERATION:
		return get_acc();

	case SENSOR_READ_TEMPERATURE:
		return get_temp();

	case SENSOR_READ_ID:
		return get_device_id();

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
bool bma222::calibrate(sensor_calibration_t caltype, int code, void *info){

	return false;
}

/**
 * \brief Initiate a sensor device software reset.
 *
 * \param   arg     Device-specific argument options.
 * \return  bool    true if the call succeeds, else false is returned.
 */
bool bma222::reset(int arg){

	return false;
}

/**
 * \brief Set a sensor device to low-power or standby mode.
 *
 * \param   arg     Device-specific argument options.
 * \return  bool    true if the call succeeds, else false is returned.
 */
bool bma222::sleep(int arg){
	sleep_en(true);
	return true;
}

/**
 * \brief Execute a sensor device control function.
 *
 * \param   cmd     Specifies the IOCTL command.
 * \param   arg     Specifies command parameters (varies by command).
 * \return  bool    true if the call succeeds, else false is returned.
 */
bool bma222::ioctl(sensor_command_t cmd, void *arg){

	switch (cmd) {
	default:
		err = SENSOR_ERR_UNSUPPORTED;
		return false;

	case SENSOR_SET_STATE:
	{
		sensor_state_t const mode = *((sensor_state_t *)arg);

		if ((mode != mod) && set_state(mode)) {
			mod = (mode == SENSOR_STATE_RESET)
					? SENSOR_STATE_NORMAL : mode;
			return true;
		} else {
			return false;
		}
	}

	case SENSOR_SET_RANGE:
		return set_range((uint16_t)*((int *)arg));

	case SENSOR_SET_BANDWIDTH:
		return set_bandwidth((uint16_t)*((int *)arg));

	case SENSOR_ENABLE_EVENT:
		return event(*((sensor_event_t *)arg), 0, true);

	case SENSOR_DISABLE_EVENT:
		return event(*((sensor_event_t *)arg), 0, false);

	case SENSOR_SET_THRESHOLD:
		return set_threshold((sensor_threshold_desc_t *)arg);

	case SENSOR_GET_THRESHOLD:
		return get_threshold((sensor_threshold_desc_t *)arg);

	case SENSOR_SET_TAP:
		return set_tap((sensor_tap_params_t *)arg);

	case SENSOR_READ_VECTOR:
		if (!get_acc()) {
			err = SENSOR_ERR_DRIVER;
			return false;
		}else {
			return true;
		}
	case SENSOR_READ_SCALAR:
		if(!get_temp()){
			err = SENSOR_ERR_DRIVER;
			return false;
		}else {
			return true;
		}
	}
}

/**
 * \brief Activate a sensor self-test function.
 *
 * \param test_code Address of a device-specific numeric test code.
 * \param arg       Device-specific self-test argument options.
 * \return bool     true if the test succeeds, else false is returned.
 */
bool bma222::selftest(int *test_code, void *arg){
	bool result = false;

	if (SENSOR_TEST_DEFLECTION == *test_code) {
		/* \todo Execute BMA222 electrostatic deflection self-test. */
	}

	return result;
}

/**
 * @brief Set the BMA222 execution mode.
 *
 * This routine sets a specified BMA222 execution state to one of the
 * following:
 *
 * SENSOR_STATE_SUSPEND or SENSOR_STATE_LOWEST_POWER
 *      The BMA222 can be put into a suspend mode to easily achieve a power
 *      consumption below 1uA.  In this mode all analog modules except for
 *      power-on reset will be disabled.  Only reads through the serial
 *      interface are supported during suspend.
 *
 *  SENSOR_STATE_SLEEP or SENSOR_STATE_LOW_POWER
 *      This option sets the BMA222 to a low-power mode.  In this mode, the
 *      device periodically wakes up, evaluates acceleration data with respect
 *      to interrupt criteria defined by the user and goes back to sleep if no
 *      interrupt has occurred using the following procedure:
 *
 *      1. Wake-up
 *
 *      2. Enable analog front-end and convert acceleration data until the
 *         low-pass filters have settled.
 *
 *      3. Enable interrupt controller and evaluate interrupt conditions.
 *         Once interrupt conditions are evaluated and no interrupt has
 *         occurred, the chip goes back to sleep.  If no interrupts are enabled,
 *         acceleration for all three axes are converted once before the chip
 *         goes back to sleep.
 *
 *      4. Sleep for the programmed duration.  Available sleep durations are
 *         2ms, 10ms, 25ms, 50ms, 100ms, 500ms, 1s, and 2s.
 *
 *  SENSOR_STATE_NORMAL or SENSOR_STATE_HIGHEST_POWER
 *      In normal mode the sensor IC data and status registers can be accessed
 *      without restriction.  The device current consumption is typically
 *      250 microamps in this state.
 *
 *  SENSOR_STATE_X_AXIS_STANDBY
 *  SENSOR_STATE_Y_AXIS_STANDBY
 *  SENSOR_STATE_Z_AXIS_STANDBY
 *      In order to optimize further power consumption of the BMA222, data
 *      evaluation of individual axes can be deactivated.
 *
 *  SENSOR_STATE_RESET
 *      This function resets the device and internal registers to the power-up
 *      default settings.
 *
 * In the wake-up mode, the BMA222 automatically switches from sleep mode to
 * normal mode after a delay defined by a programmable \c wake_up_pause value.
 * After transitioning from sleep to normal mode, acceleration data acquisition
 * and interrupt verification are performed.  The sensor automatically returns
 * to sleep mode again if no interrupt criteria are satisfied.
 *
 * @param mode      A specified sensor operational mode.
 * @return bool     true if the call succeeds, else false is returned.
 */
bool bma222::set_state(sensor_state_t mode){

		/*
		 * Perform a state transition out of the previous mode
		 * into the new mode.
		 */

		switch (mod) {
		case SENSOR_STATE_NORMAL :
		case SENSOR_STATE_HIGHEST_POWER:

			if ((SENSOR_STATE_SLEEP == mode) ||
					(SENSOR_STATE_LOW_POWER == mode)) {
				/* Enter sleep from normal mode. */
				sleep_en(true);
			} else if ((SENSOR_STATE_SUSPEND == mode) ||
					(SENSOR_STATE_LOWEST_POWER == mode)) {
				/* Enter suspend mode from normal mode. */
				bus->put(BMA222_I2C_ADDR, BMA222_POWER_MODES, BMA222_SUSPEND);
			}

			break;

		case SENSOR_STATE_SLEEP:
		case SENSOR_STATE_LOW_POWER:

			sleep_en(false);

			if ((SENSOR_STATE_SUSPEND == mode) ||
					(SENSOR_STATE_LOWEST_POWER == mode)) {
				/* Enter suspend mode from sleep mode. */
				bus->put(BMA222_I2C_ADDR, BMA222_POWER_MODES, BMA222_SUSPEND);
			}

			break;

		case SENSOR_STATE_SUSPEND:
		case SENSOR_STATE_LOWEST_POWER:

			if ((SENSOR_STATE_SLEEP == mode) ||
					(SENSOR_STATE_LOW_POWER == mode)) {
				/* Enter sleep from suspend mode. */
				sleep_en(true);
			} else if ((SENSOR_STATE_NORMAL == mode) ||
					(SENSOR_STATE_HIGHEST_POWER == mode)) {
				/* Enter normal mode from suspend mode. */
				bus->put(BMA222_I2C_ADDR, BMA222_POWER_MODES, 0);
			}

			break;

		case SENSOR_STATE_RESET:

			/*
			 * \todo
			 * Update sensor device descriptor operational settings.
			 */
			bus->put(BMA222_I2C_ADDR, BMA222_SOFTRESET, BMA222_RESET);
			bus->put(BMA222_I2C_ADDR, BMA222_SOFTRESET, BMA222_RESET);
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
bool bma222::get_state(sensor_state_t *mode){

	/*
	 * Return internal mode
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
bool bma222::get_device_id(){

	/*
	 * Get the device id
	 */
	if(sizeof(bma222_id_regs_t) != bus->read(
				BMA222_I2C_ADDR,				// Destination
				sizeof(bma222_id_regs_t),		// Size to read
				(uint8_t)BMA222_CHIP_ID,		// Memory index to read from
				(uint8_t*)&id					// Where to store the value
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
 * @brief Read BMA222 temperature data.
 *
 * This function reads temperature data, where center temperature 24 C
 * corresponds to a value 0x00 read from the temperature register with
 * temperature slope 0.5 C/LSB.
 *
 * @return bool     true if the call succeeds, else false is returned.
 */
bool bma222::get_temp(){

	// Container
	int8_t	temp_data;

	/*
	 * Get the device id
	 */
	if(sizeof(int8_t) != bus->read(
				BMA222_I2C_ADDR,				// Destination
				sizeof(int8_t),					// Size to read
				(uint8_t)BMA222_TEMP,			// Memory index to read from
				(uint8_t*)&temp_data			// Where to store the value
			)){

		/*
		 * Error present
		 */
		err = SENSOR_ERR_DRIVER;
		return false;
	}

	/*
	 * Convert
	 */
	cache.temp.temperature.value = BMA222_TEMP_OFFSET + ((int16_t)temp_data / 2);

	/*
	 * Return the bus status
	 */
	return (STATUS_OK == bus->get_status());
}

/**
 * @brief Read BMA222 acceleration data.
 *
 * This function obtains accelerometer data for all three axes of the Bosch
 * device.  The data is read from three device registers using a multi-byte
 * bus transfer.
 *
 * Along with the actual sensor data, the LSB byte contains a "new" flag
 * indicating if the data for this axis has been updated since the last
 * time the axis data was read.  Reading either LSB or MSB data will
 * clear this flag.
 *
 * @return bool     true if the call succeeds, else false is returned.
 */
bool bma222::get_acc(){

	/*
	 * Get the device id
	 */
	if(sizeof(uint8_t) != bus->read(
				BMA222_I2C_ADDR,				// Destination
				sizeof(regs.acc),				// Size to read
				hal.burst_addr,					// Memory index to read from
				(uint8_t*)&regs.acc				// Where to store the value
			)){

		/*
		 * Error present
		 */
		err = SENSOR_ERR_DRIVER;
		return false;
	}

	/*
	 * Convert
	 */
	memcpy((void*)&cache.acc.acc.axis, (void*)&regs.acc, sizeof(regs.acc));

	/*
	 * Return the bus status
	 */
	return (STATUS_OK == bus->get_status());

}

/**
 * @brief Enable or disable the BMA222 sleep mode.
 *
 * This routine enables or disables the BMA222 sleep mode depending upon
 * the value of the \sleep parameter; a \true value enables sleep mode and
 * a \false value disables sleep mode by setting or clearing the 'sleep_en'
 * bit, respectively.
 *
 * @param sleep     Set flag \true to enable sleep mode.
 * @return Nothing
 */
void bma222::sleep_en(bool sleep){

	uint8_t const power_mode_val = (sleep == true)
			? (BMA222_LOWPOWER_EN | BMA222_SLEEP_DUR_1ms) : 0;
	bus->put(BMA222_I2C_ADDR, BMA222_POWER_MODES, power_mode_val);
}

/**
 * @brief Set the BMA222 full scale acceleration range.
 *
 * @param range     The index of a driver-specific range table entry.
 * @return bool     true if the call succeeds, else false is returned.
 */
bool bma222::set_range(int16_t range){
	bus->put(BMA222_I2C_ADDR, BMA222_G_RANGE, range_table[range].reserved_val);
	return (STATUS_OK == bus->get_status());
}

/**
 * @brief Set the BMA222 digital filter cut-off frequency
 *
 * @param band     The index of a driver-specific bandwidth table entry.
 * @return bool     true if the call succeeds, else false is returned.
 */
bool bma222::set_bandwidth(int16_t band){
	bus->put(BMA222_I2C_ADDR, BMA222_BANDWIDTH, band_table[band].reserved_val);
	return (STATUS_OK == bus->get_status());
}

/**
 * @brief Set event threshold value
 *
 * @param threshold Address of threshold descriptor.
 * @return bool     true if the call succeeds, else false is returned.
 */
bool bma222::set_threshold(sensor_threshold_desc_t *threshold){

		/* Threshold values will be passed in milli-g units (assumed). */
		int32_t value = scaled_to_raw(this, threshold->value);

		switch (threshold->type) {
		default:
			return false;

		case SENSOR_THRESHOLD_MOTION:

			/*
			 * Any-Motion (slope) Threshold
			 *
			 * The slope interrupt threshold value LSB corresponds to an LSB
			 * of acceleration data for the selected g-range. The default
			 * value of 14h for the default 2mg range implies a threshold of
			 * 312.5mg.
			 */
			bus->put(BMA222_I2C_ADDR, BMA222_SLOPE_THRESHOLD, (uint8_t)value);
			break;

		case SENSOR_THRESHOLD_TAP:

			/*
			 * Single-Tap or Double-Tap Threshold
			 *
			 * An LSB of tap threshold depends upon the selected g-range
			 * where an acceleration delta of 62.5mg in 2-g, 125mg in 4-g,
			 * etc. will apply. The default 0ah raw value corresponds to the
			 * default 2mg range.
			 */
		{
			int8_t const mask = BMA222_TAP_TH_FIELD;
			bus->reg_fieldset(BMA222_I2C_ADDR, BMA222_TAP_CONFIG, mask, (uint8_t)value);
		}
		break;

		case SENSOR_THRESHOLD_LOW_G:

			/*
			 * Low-G Threshold
			 *
			 * An LSB of low-g threshold always corresponds to an
			 * acceleration of 7.81mg; namely, the increment is independent
			 * of the g-range. Divide the requested threshold in milli-g
			 * by 7.81mg (781/100) to calculate the register value.
			 */
			value = (threshold->value * 100) / 781;
			bus->put(BMA222_I2C_ADDR, BMA222_LOW_G_THRESHOLD, (uint8_t)value);
			break;

		case SENSOR_THRESHOLD_HIGH_G:

			/*
			 * High-G Threshold
			 *
			 * An LSB of high-g threshold depends upon the selected g-range
			 * where an acceleration delta of 62.5mg in 2-g, 125mg in 4-g,
			 * etc. will apply. The default 0ah raw value corresponds to the
			 * default 2mg range.
			 */
			bus->put(BMA222_I2C_ADDR, BMA222_HIGH_G_THRESHOLD, (uint8_t)value);
			break;
		}

		return (STATUS_OK == bus->get_status());
}

/**
 * @brief Get event threshold value
 *
 * @param threshold Address of threshold descriptor.
 * @return bool     true if the call succeeds, else false is returned.
 */
bool bma222::get_threshold(sensor_threshold_desc_t *threshold){

	switch (threshold->type) {
	default:
		return false;

	case SENSOR_THRESHOLD_MOTION:
		threshold->value = raw_to_scaled(this,
				bus->get(BMA222_I2C_ADDR, BMA222_SLOPE_THRESHOLD));
		break;

	case SENSOR_THRESHOLD_TAP:
	{
		uint8_t const mask = BMA222_TAP_TH_FIELD;
		threshold->value = raw_to_scaled(this,
				bus->reg_fieldget(BMA222_I2C_ADDR, BMA222_TAP_CONFIG,
				mask));
	}
	break;

	case SENSOR_THRESHOLD_LOW_G:
		threshold->value = (781 / 100) * bus->get(BMA222_I2C_ADDR,
				BMA222_LOW_G_THRESHOLD);
		break;

	case SENSOR_THRESHOLD_HIGH_G:
		threshold->value = raw_to_scaled(this,
				bus->get(BMA222_I2C_ADDR, BMA222_HIGH_G_THRESHOLD));
		break;
	}

	return (STATUS_OK == bus->get_status());
}

/**
 * @brief BMA222 tap detection configuration
 *
 * @param params    Address of an initialized tap parameter structure.
 * @return bool     true if the call succeeds, else false is returned.
 */
bool bma222::set_tap(sensor_tap_params_t *params){

	/* \todo Implement the device tap functions. */

	return false;
}

/**
 * @brief Enable or disable BMA222 sensor events.
 *
 * @param  sensor_event Specifies the sensor event type
 * @param  callback     Application-defined event callback handler descriptor
 * @param  enable       Enable flag: true = enable event, false = disable event
 * @return bool         true if the call succeeds, else false is returned
 */
bool bma222::event(sensor_event_t sensor_event,
		sensor_event_callback_t* callback, bool enable){

		bool status = false;

		uint8_t int_enable1 = bus->get(BMA222_I2C_ADDR, BMA222_16_INTR_EN);
		uint8_t int_enable2 = bus->get(BMA222_I2C_ADDR, BMA222_17_INTR_EN);

		if (sensor_event & SENSOR_EVENT_NEW_DATA) {
			if (callback) {
				callbacks[0] = *callback;
			}

			if (enable) {
				int_enable2 |=  BMA222_DATA_EN;
			} else {
				int_enable2 &= ~BMA222_DATA_EN;
			}

			status = true;
		}

		if (sensor_event & SENSOR_EVENT_MOTION) {
			if (callback) {
				callbacks[1] = *callback;
			}

			if (enable) {
				/* Enable slope detection on x, y, and z axes using the
				 * default settings for the slope threshold & duration.
				 */
				int_enable1 |=  (BMA222_SLOPE_EN_Z | BMA222_SLOPE_EN_Y |
						BMA222_SLOPE_EN_X);
			} else {
				int_enable1 &= ~(BMA222_SLOPE_EN_Z | BMA222_SLOPE_EN_Y |
						BMA222_SLOPE_EN_X);
			}

			status = true;
		}

		if (sensor_event & SENSOR_EVENT_LOW_G) {
			if (callback) {
				callbacks[2] = *callback;
			}

			if (enable) {
				int_enable2 |=  BMA222_LOW_EN;
			} else {
				int_enable2 &= ~BMA222_LOW_EN;
			}

			status = true;
		}

		if (sensor_event & SENSOR_EVENT_HIGH_G) {
			if (callback) {
				callbacks[3] = *callback;
			}

			if (enable) {
				/* Enable high-g detection on x, y, and z axes using the
				 * default settings for the high-g duration &
				 * hysteresis.
				 */
				int_enable2 |=  (BMA222_HIGH_EN_Z | BMA222_HIGH_EN_Y |
						BMA222_HIGH_EN_X);
			} else {
				int_enable2 &= ~(BMA222_HIGH_EN_Z | BMA222_HIGH_EN_Y |
						BMA222_HIGH_EN_X);
			}

			status = true;
		}

		if (sensor_event & SENSOR_EVENT_TAP) {
			if (callback) {
				callbacks[4] = *callback;
			}

			if (enable) {
				int_enable1 |=  (BMA222_S_TAP_EN | BMA222_D_TAP_EN);
			} else {
				int_enable1 &= ~(BMA222_S_TAP_EN | BMA222_D_TAP_EN);
			}

			status = true;
		}

		bus->put(BMA222_I2C_ADDR, BMA222_16_INTR_EN, int_enable1);
		bus->put(BMA222_I2C_ADDR, BMA222_17_INTR_EN, int_enable2);

		return status;
}


/**
 * @brief Bosch BMA222 driver interrupt service routine.
 *
 * This is the common interrupt service routine for all enabled BMA222 interrupt
 * events.  Five different types of interrupts can be programmed.  All interrupt
 * criteria are combined and drive the interrupt pad with a Boolean \c OR
 * condition.
 *
 * Interrupt criteria are tested against values from the BMA222 digital filter
 * output.  All thresholds are scaled using the current device range.  Timings
 * for high and low acceleration are absolute values (1 LSB of HG_dur and LG_dur
 * registers corresponds to 1 millisecond, +/- 10%).  Timing for the any-motion
 * interrupt and alert detection are proportional to the bandwidth setting.
 *
 * This routine handles interrupts generated when low-g, high-g, any-motion,
 * alert, and new data criteria are satisfied and the corresponding event
 * notification is enabled in the device.
 *
 * The BMA222 device does not provide any way to definitively identify an
 * any-motion interrupt once it has occurred.  So, if a handler has been
 * installed for that event, it will always be called by this routine,
 * and the SENSOR_EVENT_MOTION indicator will be set in the event type field.
 *
 * @param sensor	The sensor to address
 * @param arg       The default args
 * @return Nothing.
 */
void bma222::isr(void* sensor, void *arg){
#ifdef BMA222_IRQ

	/*
	 * NOTE !!! This code has not been updated and may pose issues
	 * in the compile process...
	 *
	 */


	/*
	 * Convert sensor
	 */
	bma222_t*	bma222 = (bma222_t*)sensor;

	/*
	 * Read what event has happened
	 */
	bus->read(	BMA222_I2C_ADDR,
				sizeof(bma222_event_regs_t),
				hal.burst_addr,
				(uint8_t*)&regs);

	/*
	 * If the bus is in a good state we know the transaction was
	 * a success.
	 */
	if (STATUS_OK == bus->get_status()) {

		/*
		 * Get timestamp
		 */
		bma222->evt_data.data.timestamp = timestamp();
		bma222->evt_data.event = SENSOR_EVENT_UNKNOWN;

		if (bma222->regs.status_field.data_int) {
			bma222->evt_data.event |= SENSOR_EVENT_NEW_DATA;
			(callbacks[0].handler)(&bma222->evt_data, callbacks[0].arg);
		}

		if (bma222->regs.status_field.slope_int) {
			bma222->evt_data.event |= SENSOR_EVENT_MOTION;
			(callbacks[1].handler)(&bma222->evt_data, callbacks[1].arg);
		}

		if (bma222->regs.status_field.low_int) {
			bma222->evt_data.event |= SENSOR_EVENT_LOW_G;
			(callbacks[2].handler)(&bma222->evt_data, callbacks[2].arg);
		}

		if (bma222->regs.status_field.high_int) {
			bma222->evt_data.event |= SENSOR_EVENT_HIGH_G;
			(callbacks[3].handler)(&bma222->evt_data, callbacks[3].arg);
		}

		if (bma222->regs.status_field.s_tap_int) {
			bma222->evt_data.event |= SENSOR_EVENT_S_TAP;
			(callbacks[4].handler)(&bma222->evt_data, callbacks[4].arg);
		}

		if (bma222->regs.status_field.d_tap_int) {
			bma222->evt_data.event |= (sensor_event_t)SENSOR_EVENT_D_TAP;
			(callbacks[4].handler)(&bma222->evt_data, callbacks[4].arg);
		}
	}
#endif
}

/*
 * bma222.h
 *
 *  Created on: Jul 7, 2015
 *      Author: fpapinea
 */

#ifndef PLATFORM_SENSOR_DRIVERS_BOSCH_BMA222_H_
#define PLATFORM_SENSOR_DRIVERS_BOSCH_BMA222_H_

#include <platform/sensor/i2c/sensor_i2c.h>

/* TWI/I2C address (write @ 0x16 on bus, read @ 0x17 on bus) */
#define BMA222_I2C_ADDR         (0x18)
#define BMA222_SPI_MODE         (3)

/* Sensor Data Resolution and Offsets */
#define BMA222_DATA_RESOLUTION  (8)     /* signed axis data size (bits) */
#define BMA222_TEMP_OFFSET      (24)    /* temperature center (Celsius) */
#define BMA222_INT_PIN			(13)

#define BMA222_TRANSACTION_BYTE		(BMA222_DATA_RESOLUTION / 8)

/*
 * Standard Register Addresses (TWI & SPI)
 *
 * w/r=write/read, wo=write only, ro=read only
 */
typedef enum {
	BMA222_CHIP_ID = 0,     	/* 00 (ro) chip ID (always 0x03) */
	BMA222_01_RSVD,         	/* 01 reserved */
	BMA222_NEW_DATA_X,      	/* 02 (ro) X-axis new data flag */
	BMA222_ACC_X,           	/* 03 (ro) X-axis acceleration */
	BMA222_NEW_DATA_Y,      	/* 04 (ro) Y-axis new data flag */
	BMA222_ACC_Y,           	/* 05 (ro) Y-axis acceleration */
	BMA222_NEW_DATA_Z,      	/* 06 (ro) Z-axis new data flag */
	BMA222_ACC_Z,           	/* 07 (ro) Z-axis acceleration */
	BMA222_TEMP,            	/* 08 (ro) temperature */
	BMA222_INTR_STATUS,     	/* 09 (ro) misc. interrupt status */
	BMA222_NEW_DATA_STATUS, 	/* 0a (ro) new data interrupt status */
	BMA222_TAP_SLOPE_STATUS, 	/* 0b (ro) tap and slope interrupt status */
	BMA222_ORIENTATION_STATUS,	/* 0c (ro) flat and orientation status */
	BMA222_0D_RSVD,         	/* 0d reserved */
	BMA222_0E_RSVD,         	/* 0e reserved */
	BMA222_G_RANGE,         	/* 0f (w/r) G-range selection */
	BMA222_BANDWIDTH,       	/* 10 (w/r) bandwidth selection */
	BMA222_POWER_MODES,     	/* 11 (w/r) power mode configuration */
	BMA222_12_RSVD,         	/* 12 reserved */
	BMA222_DATA_HIGH_BW,    	/* 13 (w/r) acceleration data filter */
	BMA222_SOFTRESET,       	/* 14 (wo) user-triggered software reset */
	BMA222_15_RSVD,         	/* 15 reserved */
	BMA222_16_INTR_EN,      	/* 16 (w/r) interrupt enable bits */
	BMA222_17_INTR_EN,      	/* 17 (w/r) interrupt enable bits */
	BMA222_18_RSVD,         	/* 18 reserved */
	BMA222_19_INTR_MAP,     	/* 19 (w/r) interrupt pin mapping */
	BMA222_1A_INTR_MAP,     	/* 1a (w/r) interrupt pin mapping */
	BMA222_1B_INTR_MAP,     	/* 1b (w/r) interrupt pin mapping */
	BMA222_1C_RSVD,         	/* 1c reserved */
	BMA222_1D_RSVD,         	/* 1d reserved */
	BMA222_INTR_DATA_SRC,   	/* 1e (w/r) filtered/unfiltered data */
	BMA222_1F_RSVD,         	/* 1f reserved */
	BMA222_INTR_PIN_CONFIG, 	/* 20 (w/r) interrupt pin configuration */
	BMA222_INTR_PIN_MODE,   	/* 21 (w/r) interrupt pin mode & reset */
	BMA222_LOW_G_DURATION,  	/* 22 (w/r) low-g interrupt delay time */
	BMA222_LOW_G_THRESHOLD, 	/* 23 (w/r) low-g interrupt threshold */
	BMA222_EVENT_HYSTERESIS, 	/* 24 (w/r) low-/high-g event hysteresis */
	BMA222_HIGH_G_DURATION, 	/* 25 (w/r) high-g interrupt delay time */
	BMA222_HIGH_G_THRESHOLD, 	/* 26 (w/r) high-g interrupt threshold */
	BMA222_SLOPE_DURATION,  	/* 27 (w/r) no. samples for slope event */
	BMA222_SLOPE_THRESHOLD, 	/* 28 (w/r) slope event threshold */
	BMA222_29_RSVD,         	/* 29 reserved */
	BMA222_TAP_TIMING,      	/* 2a (w/r) single/double tap event timing */
	BMA222_TAP_CONFIG,      	/* 2b (w/r) wake samples & thresholds */
	BMA222_ORIENTATION_CONFIG, 	/* 2c (w/r) hysteresis, blocking, & mode */
	BMA222_ORIENTATION_THETA, 	/* 2d (w/r) theta blocking angle */
	BMA222_FLAT_THETA,      	/* 2e (w/r) flat threshold angle */
	BMA222_FLAT_HOLD_TIME,  	/* 2f (w/r) flat hold time */
	BMA222_30_RSVD,         	/* 30 reserved */
	BMA222_31_RSVD,         	/* 31 reserved */
	BMA222_SENSOR_SELF_TEST, 	/* 32 (w/r) self-test settings/activation */
	BMA222_EEPROM_CONTROL,  	/* 33 (w/r) non-volatile memory control */
	BMA222_DIGITAL_IO_CONTROL, 	/* 34 (w/r) I2C & SPI interface settings */
	BMA222_35_RSVD,         	/* 35 reserved */
	BMA222_FAST_OFFSET_COMP, 	/* 36 (w/r) fast offset compensation settings */
	BMA222_SLOW_OFFSET_COMP, 	/* 37 (w/r) slow offset compensation settings */
	BMA222_OFFSET_FILT_X,   	/* 38 (w/r) filtered data compensation x-axis */
	BMA222_OFFSET_FILT_Y,   	/* 39 (w/r) filtered data compensation y-axis */
	BMA222_OFFSET_FILT_Z,   	/* 3a (w/r) filtered data compensation z-axis */
	BMA222_OFFSET_UNFILT_X, 	/* 3b (w/r) unfiltered data compensation x-axis */
	BMA222_OFFSET_UNFILT_Y, 	/* 3c (w/r) unfiltered data compensation y-axis */
	BMA222_OFFSET_UNFILT_Z  	/* 3d (w/r) unfiltered data compensation z-axis */
} bma222_register_t;

/** \brief BMA222 Register Bit Definitions */
/** @{ */

/* BMA222_CHIP_ID (0x00) */

#define BMA222_ID_VAL           (0x03)

/* BMA222_INTR_STATUS (0x09) */

#define BMA222_FLAT_INT         (1 << 7)    /* flat interrupt status */
#define BMA222_ORIENT_INT       (1 << 6)    /* orientation interrupt status */
#define BMA222_S_TAP_INT        (1 << 5)    /* single tap interrupt status */
#define BMA222_D_TAP_INT        (1 << 4)    /* double tap interrupt status */
#define BMA222_SLOPE_INT        (1 << 2)    /* slope interrupt status */
#define BMA222_HIGH_INT         (1 << 1)    /* high-g interrupt status */
#define BMA222_LOW_INT          (1 << 0)    /* low-g interrupt status */

/* BMA222_NEW_DATA_STATUS (0x0a) */

#define BMA222_DATA_INT         (1 << 7)    /* new data interrupt status */

/* BMA222_TAP_SLOPE_STATUS (0x0b) */

#define BMA222_TAP_SIGN_POS     (1 << 7)    /* tap interrupt sign (0=negative) */
#define BMA222_TAP_FIRST_Z      (1 << 6)    /* z-axis triggered tap interrupt */
#define BMA222_TAP_FIRST_Y      (1 << 5)    /* y-axis triggered tap interrupt */
#define BMA222_TAP_FIRST_X      (1 << 4)    /* x-axis triggered tap interrupt */
#define BMA222_SLOPE_SIGN_POS   (1 << 3)    /* slope interrupt sign (0=negative) */
#define BMA222_SLOPE_FIRST_Z    (1 << 2)    /* z-axis triggered slope interrupt */
#define BMA222_SLOPE_FIRST_Y    (1 << 1)    /* y-axis triggered slope interrupt */
#define BMA222_SLOPE_FIRST_X    (1 << 0)    /* x-axis triggered slope interrupt */

/* BMA222_ORIENTATION_STATUS (0x0c) */

#define BMA222_FLAT             (1 << 7)    /* flat condition is fulfilled */
#define BMA222_Z_DOWN           (1 << 6)    /* z-axis orientation (0=upward) */
#define BMA222_XY_PORTRAIT_UP   (0 << 4)    /* x-y plane portrait upright */
#define BMA222_XY_PORTRAIT_DOWN (1 << 4)    /* x-y plane portrait upside-down */
#define BMA222_XY_LANDSCAPE_L   (2 << 4)    /* x-y plane landscape left */
#define BMA222_XY_LANDSCAPE_R   (3 << 4)    /* x-y plane landscape right */
#define BMA222_HIGH_SIGN_NEG    (1 << 3)    /* high-g interrupt sign * (0=positive) */
#define BMA222_HIGH_FIRST_Z     (1 << 2)    /* z-axis triggered high-g interrupt */
#define BMA222_HIGH_FIRST_Y     (1 << 1)    /* y-axis triggered high-g interrupt */
#define BMA222_HIGH_FIRST_X     (1 << 0)    /* x-axis triggered high-g interrupt */

/* BMA222_G_RANGE (0x0f) */

#define BMA222_RANGE_2G         (0x03)      /* +/- 2g range (default) */
#define BMA222_RANGE_4G         (0x05)      /* +/- 4g range */
#define BMA222_RANGE_8G         (0x08)      /* +/- 8g range */
#define BMA222_RANGE_16G        (0x0c)      /* +/- 16g range */

/* BMA222_BANDWIDTH (0x10) */

#define BMA222_BANDWIDTH_8Hz    (0x08)      /*   7.81 Hz filtered data bandwidth */
#define BMA222_BANDWIDTH_16Hz   (0x09)      /*  15.63 Hz filtered data bandwidth */
#define BMA222_BANDWIDTH_31Hz   (0x0a)      /*  31.25 Hz filtered data bandwidth */
#define BMA222_BANDWIDTH_63Hz   (0x0b)      /*  62.5 Hz filtered data bandwidth */
#define BMA222_BANDWIDTH_125Hz  (0x0c)      /* 125 Hz filtered data bandwidth */
#define BMA222_BANDWIDTH_250Hz  (0x0d)      /* 250 Hz filtered data bandwidth */
#define BMA222_BANDWIDTH_500Hz  (0x0e)      /* 500 Hz filtered data bandwidth */
#define BMA222_BANDWIDTH_1000Hz (0x1f)      /* 1000 Hz filtered data bandwidth */

/* BMA222_POWER_MODES (0x11) */

#define BMA222_SUSPEND          (1  << 7)   /* set suspend mode (0=reset mode) */
#define BMA222_LOWPOWER_EN      (1  << 6)   /* set low-power mode (0=reset mode) */
#define BMA222_SLEEP_DUR_0_5ms  (5  << 1)   /* 0.5 ms sleep phase duration */
#define BMA222_SLEEP_DUR_1ms    (6  << 1)   /*   1 ms sleep phase duration */
#define BMA222_SLEEP_DUR_2ms    (7  << 1)   /*   2 ms sleep phase duration */
#define BMA222_SLEEP_DUR_4ms    (8  << 1)   /*   4 ms sleep phase duration */
#define BMA222_SLEEP_DUR_6ms    (9  << 1)   /*   6 ms sleep phase duration */
#define BMA222_SLEEP_DUR_10ms   (10 << 1)   /*   6 ms sleep phase duration */
#define BMA222_SLEEP_DUR_25ms   (11 << 1)   /*  25 ms sleep phase duration */
#define BMA222_SLEEP_DUR_50ms   (12 << 1)   /*  50 ms sleep phase duration */
#define BMA222_SLEEP_DUR_100ms  (13 << 1)   /* 100 ms sleep phase duration */
#define BMA222_SLEEP_DUR_500ms  (14 << 1)   /* 500 ms sleep phase duration */
#define BMA222_SLEEP_DUR_1000ms (15 << 1)   /*   1 s sleep phase duration */

/* BMA222_DATA_HIGH_BW (0x13) */

#define BMA222_DATA_UNFILTERED  (1 << 7)    /* unfiltered data (0=filtered) */

/* BMA222_SOFTRESET (0x14) */

#define BMA222_RESET            (0xb6)      /* user-triggered reset write value */

/* BMA222_16_INTR_EN (0x16) */

#define BMA222_FLAT_EN          (1 << 7)    /* flat interrupt enable */
#define BMA222_ORIENT_EN        (1 << 6)    /* orientation interrupt enable */
#define BMA222_S_TAP_EN         (1 << 5)    /* single tap interrupt enable */
#define BMA222_D_TAP_EN         (1 << 4)    /* double tap interrupt enable */
#define BMA222_SLOPE_EN_Z       (1 << 2)    /* z-axis slope interrupt enable */
#define BMA222_SLOPE_EN_Y       (1 << 1)    /* y-axis slope interrupt enable */
#define BMA222_SLOPE_EN_X       (1 << 0)    /* x-axis slope interrupt enable */

/* BMA222_17_INTR_EN (0x17) */

#define BMA222_DATA_EN          (1 << 4)    /* new data interrupt enable */
#define BMA222_LOW_EN           (1 << 3)    /* low-g interrupt enable */
#define BMA222_HIGH_EN_Z        (1 << 2)    /* z-axis high-g interrupt enable */
#define BMA222_HIGH_EN_Y        (1 << 1)    /* y-axis high-g interrupt enable */
#define BMA222_HIGH_EN_X        (1 << 0)    /* x-axis high-g interrupt enable */

/* BMA222_19_INTR_MAP (0x19) */

#define BMA222_INT1_FLAT        (1 << 7)    /* map flat interrupt to INT1 */
#define BMA222_INT1_ORIENT      (1 << 6)    /* map orientation interrupt to INT1 */
#define BMA222_INT1_S_TAP       (1 << 5)    /* map single tap interrupt to INT1 */
#define BMA222_INT1_D_TAP       (1 << 4)    /* map double tap interrupt to INT1 */
#define BMA222_INT1_SLOPE       (1 << 2)    /* map slope interrupt to INT1 */
#define BMA222_INT1_HIGH        (1 << 1)    /* map high-g interrupt to INT1 */
#define BMA222_INT1_LOW         (1 << 0)    /* map low-g interrupt to INT1 */

/* BMA222_1A_INTR_MAP (0x1a) */

#define BMA222_INT2_DATA        (1 << 7)    /* map new data interrupt to INT2 */
#define BMA222_INT1_DATA        (1 << 0)    /* map new data interrupt to INT1 */

/* BMA222_1B_INTR_MAP (0x1b) */

#define BMA222_INT2_FLAT        (1 << 7)    /* map flat interrupt to INT2 */
#define BMA222_INT2_ORIENT      (1 << 6)    /* map orientation interrupt to INT2 */
#define BMA222_INT2_S_TAP       (1 << 5)    /* map single tap interrupt to INT2 */
#define BMA222_INT2_D_TAP       (1 << 4)    /* map double tap interrupt to INT2 */
#define BMA222_INT2_SLOPE       (1 << 2)    /* map slope interrupt to INT2 */
#define BMA222_INT2_HIGH        (1 << 1)    /* map high-g interrupt to INT2 */
#define BMA222_INT2_LOW         (1 << 0)    /* map low-g interrupt to INT2 */

/* BMA222_INTR_DATA_SRC (0x1e) */

#define BMA222_INT_SRC_DATA     (1 << 5)    /* unfiltered new data interrupt */
#define BMA222_INT_SRC_TAP      (1 << 4)    /* unfiltered s/d tap interrupt data */
#define BMA222_INT_SRC_SLOPE    (1 << 2)    /* unfiltered slope interrupt data */
#define BMA222_INT_SRC_HIGH     (1 << 1)    /* unfiltered high-g interrupt data */
#define BMA222_INT_SRC_LOW      (1 << 0)    /* unfiltered low-g interrupt data */

/* BMA222_INTR_PIN_CONFIG (0x20) */

#define BMA222_INT2_OD          (1 << 3)    /* open drive for INT2 pin */
#define BMA222_INT2_LVL_1       (1 << 2)    /* active level 1 for INT2 (default) */
#define BMA222_INT1_OD          (1 << 1)    /* open drive for INT1 pin */
#define BMA222_INT1_LVL_1       (1 << 0)    /* active level 1 for INT1 (default) */

/* BMA222_INTR_PIN_MODE (0x21) */

#define BMA222_RESET_INT        (0x80)      /* reset any latch interrupt */
#define BMA222_INT_NON_LATCHED  (0x00)      /* non-latch interrupt (default) */
#define BMA222_INT_TMP_250ms    (0x01)      /* 250ms temporary latch interrupt */
#define BMA222_INT_TMP_500ms    (0x02)      /* 500ms temporary latch interrupt */
#define BMA222_INT_TMP_1sec     (0x03)      /* 1000ms temporary latch interrupt */
#define BMA222_INT_TMP_2sec     (0x04)      /* 2000ms temporary latch interrupt */
#define BMA222_INT_TMP_4sec     (0x05)      /* 4000ms temporary latch interrupt */
#define BMA222_INT_TMP_8sec     (0x06)      /* 8000ms temporary latch interrupt */
#define BMA222_INT_TMP_500us    (0x0a)      /* 500us temporary latch interrupt */
#define BMA222_INT_TMP_1ms      (0x0b)      /* 1ms temporary latch interrupt */
#define BMA222_INT_TMP_12_5ms   (0x0c)      /* 12.5ms temporary latch interrupt */
#define BMA222_INT_TMP_25ms     (0x0d)      /* 25ms temporary latch interrupt */
#define BMA222_INT_TMP_50ms     (0x0e)      /* 50ms temporary latch interrupt */
#define BMA222_INT_LATCHED      (0x0f)      /* latch interrupt mode */

/**
 * BMA222_LOW_G_DURATION (0x22)
 *
 * Low-G interrupt delay time constants where the physical delay time is
 * computed as: delay[ms] = [low_dur + 1] * 2ms. The default value is 0x09
 * corresponding to a delay of 20ms.
 */

/**
 * BMA222_LOW_G_THRESHOLD (0x23)
 *
 * The log-g interrupt threshold value LSB corresponds to an acceleration
 * of 7.81mg with range 0 to 1.992g. The default value is 0x30 corresponding
 * to 375mg.
 */

/* BMA222_TAP_CONFIG (0x2b) */

#define BMA222_TAP_TH_FIELD     (0x1f)      /* tap interrupt threshold field */
#define BMA222_TAP_SAMP_FIELD   (0xc0)      /* tap wake-up samples count field */

/* BMA222_SENSOR_SELF_TEST (0x32) */

#define BMA222_SELF_TEST_NONE   (0x00)      /* no self-test (default) */
#define BMA222_SELF_TEST_AXIS_X (0x01)      /* self-test positive x-axis */
#define BMA222_SELF_TEST_AXIS_Y (0x02)      /* self-test positive y-axis */
#define BMA222_SELF_TEST_AXIS_Z (0x03)      /* self-test positive z-axis */

/** @} */

/**
 * convert real G values into register values
 *  \note no range checking is included
 */
#define threshold_in_g(threshold, range)    (((threshold) * 16) / (range))

/**
 * convert real G values into hysteresis register values
 *  \note no range checking is included
 */
#define hysteresis_in_g(hysteresis, range)  (((threshold) * 4) / (range))

/** \brief Sensor Event Registers */
typedef struct {

	uint8_t acc[3];                	   /**< Acceleration data */
	int8_t temp;                       /**< Temperature data */

	union {
		uint8_t status_byte[4];        /**< Status bytes */
		struct {                       /**< Status fields */
			uint8_t flat_int      : 1; /**< Flat interrupt triggered */
			uint8_t orient_int    : 1; /**< Orientation interrupt triggered */
			uint8_t s_tap_int     : 1; /**< Single-tap interrupt triggered */
			uint8_t d_tap_int     : 1; /**< double-tap interrupt triggered */
			uint8_t reserved_09   : 1;
			uint8_t slope_int     : 1; /**< Slope criteria triggered */
			uint8_t high_int      : 1; /**< High-g criteria triggered */
			uint8_t low_int       : 1; /**< Low-g criteria triggered */

			uint8_t data_int      : 1; /**< New data interrupt triggered */
			uint8_t reserved_0a   : 7;

			uint8_t tap_sign      : 1; /**< Tap axis motion direction */
			uint8_t tap_first_z   : 1; /**< z-axis tap interrupt */
			uint8_t tap_first_y   : 1; /**< y-axis tap interrupt */
			uint8_t tap_first_x   : 1; /**< x-axis tap interrupt */
			uint8_t slope_sign    : 1; /**< Axis motion direction */
			uint8_t slope_first_z : 1; /**< z-axis any-motion interrupt */
			uint8_t slope_first_y : 1; /**< y-axis any-motion interrupt */
			uint8_t slope_first_x : 1; /**< x-axis any-motion interrupt */

			uint8_t flat          : 1; /**< Orientation with respect
			                            * to gravity */
			uint8_t orient        : 3; /**< orientation with respect
			                            * to gravity */
			uint8_t high_sign     : 1; /**< High-g interrupt sign */
			uint8_t high_first_z  : 1; /**< z-axis high-g interrupt */
			uint8_t high_first_y  : 1; /**< y-axis high-g interrupt */
			uint8_t high_first_x  : 1; /**< x-axis high-g interrupt */
		} status_field;
	};
} bma222_event_regs_t;

/** \brief Sensor id regs **/
typedef struct {

	uint8_t dev_id;						/**< Device id */

}bma222_id_regs_t;

/*
 * Self test interface
 */
typedef struct {

	int test_code;
	void* arg;
} bma222_self_test_t;

/**
 * @brief BMA222 Cache
 */
typedef struct {
	bma222_data_t					acc;
	bma222_data_t					temp;
}bma222_cache_t;

/** @brief Data definition */
typedef sensor_data_t bma222_data_t;

/**
 * \brief The BMA222 I2C Bus Interface
 *
 * 	This it the bus driver interface class that incorporates all necessary
 * 	bus structures and needed definitions.
 *
 * 	@extends sensor_i2c_t
 */
class bma222: private sensor_i2c_t {

	/*
	 * Public class attributes
	 */
	public:

	/*
	 * Data
	 */
	bma222_cache_t 						datacache;


	/*
	 * Private class attributes
	 */
	private:

		/*
		 * Attributes
		 */
		bma222_self_test_t				test;
		bma222_event_regs_t				regs;
		bma222_id_regs_t				id;

		/*
		 * Event Attributes
		 */
		sensor_event_data_t				evt_data;
		sensor_event_callback_t			callbacks	[5];
		/*
		 * Bandwidth tables
		 */
		sensor_map_t 					range_table	[4];
		sensor_map_t					band_table	[8];

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
		bma222(bus_i2c_t* iface);

		/*!
		 *\brief The default deconstructor for the class.
		 *
		 * Nothing is done in the constructor as this class is mearly an
		 * interface to the sensor class.
		 */
		~bma222(){};

		/**
		 * \brief Updates the internal cache of the sensor.
		 *
		 * In this function, the system reads the newest data from the sensor and
		 * updates the internal caches that are mapped to the internal system BIOS.
		 * We do this to obtain a transparency between the BIOS and the sensor.
		 *
		 * @return	bool 	true of the update was successful.
		 */
		bool update();

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
		 * @brief Read BMA222 temperature data.
		 *
		 * This function reads temperature data, where center temperature 24 C
		 * corresponds to a value 0x00 read from the temperature register with
		 * temperature slope 0.5 C/LSB.
		 *
		 * @return bool     true if the call succeeds, else false is returned.
		 */
		bool get_temp();

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
		bool get_acc();

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
		void sleep_en(bool sleep);

		/**
		 * @brief Set the BMA222 full scale acceleration range.
		 *
		 * @param range     The index of a driver-specific range table entry.
		 * @return bool     true if the call succeeds, else false is returned.
		 */
		bool set_range(int16_t range);

		/**
		 * @brief Set the BMA222 digital filter cut-off frequency
		 *
		 * @param band     The index of a driver-specific bandwidth table entry.
		 * @return bool     true if the call succeeds, else false is returned.
		 */
		bool set_bandwidth(int16_t band);

		/**
		 * @brief Set event threshold value
		 *
		 * @param threshold Address of threshold descriptor.
		 * @return bool     true if the call succeeds, else false is returned.
		 */
		bool set_threshold(sensor_threshold_desc_t *threshold);

		/**
		 * @brief Get event threshold value
		 *
		 * @param threshold Address of threshold descriptor.
		 * @return bool     true if the call succeeds, else false is returned.
		 */
		bool get_threshold(sensor_threshold_desc_t *threshold);

		/**
		 * @brief BMA222 tap detection configuration
		 *
		 * @param params    Address of an initialized tap parameter structure.
		 * @return bool     true if the call succeeds, else false is returned.
		 */
		bool set_tap(sensor_tap_params_t *params);

		/**
		 * @brief Enable or disable BMA222 sensor events.
		 *
		 * @param  sensor_event Specifies the sensor event type
		 * @param  callback     Application-defined event callback handler descriptor
		 * @param  enable       Enable flag: true = enable event, false = disable event
		 * @return bool         true if the call succeeds, else false is returned
		 */
		bool event(sensor_event_t sensor_event,
				sensor_event_callback_t *callback, xdc_Bool enable);


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
		static void isr(void* sensor, void *arg);
};

/**< @brief Typedef */
typedef bma222 bma222_t;

#endif /* PLATFORM_SENSOR_DRIVERS_BOSCH_BMA222_H_ */

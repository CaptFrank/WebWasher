/*
 * sensor.h
 *
 *  Created on: Jul 6, 2015
 *      Author: francis-ccs
 */

#ifndef PLATFORM_SENSOR_SENSOR_SENSOR_H_
#define PLATFORM_SENSOR_SENSOR_SENSOR_H_

#include <configs.h>
#include "physics/physics.h"
#include <platform/bus/bus.h>
//#include <service/services/coms/coms.h>

/** \brief Sensor Type Constants */
typedef enum {
	SENSOR_TYPE_UNKNOWN       = 0x0000, /**< Unknown or unspecified sensor */
	SENSOR_TYPE_ACCELEROMETER = 0x0001, /**< Single- or multi-axis
	                                     * accelerometer */
	SENSOR_TYPE_BAROMETER     = 0x0002, /**< Atmospheric air pressure */
	SENSOR_TYPE_COMPASS       = 0x0004,  /**< Single- or multi-axis compass */
	SENSOR_TYPE_GYROSCOPE     = 0x0008,  /**< Single- or multi-axis gyroscope */
	SENSOR_TYPE_HUMIDITY      = 0x0010,  /**< Moisture or humidity sensor */
	SENSOR_TYPE_LIGHT         = 0x0020,  /**< Ambient light sensor */
	SENSOR_TYPE_MAGNETIC      = 0x0040,  /**< Magnetic sensor */
	SENSOR_TYPE_PRESSURE      = 0x0080,  /**< Pressure sensor */
	SENSOR_TYPE_PROXIMITY     = 0x0100,  /**< Proximity sensor */
	SENSOR_TYPE_TEMPERATURE   = 0x0200,  /**< Single-function temperature */
	SENSOR_TYPE_VOLTAGE       = 0x0400,  /**< Single-function voltage */
} sensor_type_t;

/** \brief Sensor Feature Constants */
typedef enum {
	SENSOR_CAPS_UNKNOWN       = 0x0000,
	SENSOR_CAPS_1_AXIS        = 0x0001,
	SENSOR_CAPS_2_AXIS        = 0x0002,
	SENSOR_CAPS_3_AXIS        = 0x0004,
	SENSOR_CAPS_SELFTEST      = 0x0008,
	SENSOR_CAPS_AUX_TEMP      = 0x0010,
	SENSOR_CAPS_HI_G_EVENT    = 0x0020,
	SENSOR_CAPS_LO_G_EVENT    = 0x0040,
	SENSOR_CAPS_TAP_EVENT     = 0x0080,
	SENSOR_CAPS_S_TAP_EVENT   = 0x0080,
	SENSOR_CAPS_D_TAP_EVENT   = 0x0100,
	SENSOR_CAPS_TILT_EVENT    = 0x0200,
	SENSOR_CAPS_DROP_EVENT    = 0x0400,
	SENSOR_CAPS_AUTO_CAL      = 0x0800,
	SENSOR_CAPS_AUX_ACCEL     = 0x1000,
	SENSOR_CAPS_TEMPERATURE   = 0x1001,
	SENSOR_CAPS_VOLTAGE		  = 0x1002,
} sensor_feature_t;

/** \brief Sensor Vendor Constants */
typedef enum {
	SENSOR_VENDOR_UNKNOWN,          /**< Unknown sensor vendor */
	SENSOR_VENDOR_ADI,              /**< Analog Devices, Inc. */
	SENSOR_VENDOR_AKM,              /**< Asahi Kasei Microdevices */
	SENSOR_VENDOR_AVAGO,            /**< Avago Technologies */
	SENSOR_VENDOR_BOSCH,            /**< Bosch Sensortec GmbH */
	SENSOR_VENDOR_HONEYWELL,        /**< Honeywell International, Inc. */
	SENSOR_VENDOR_INTERSIL,         /**< Intersil Corporation */
	SENSOR_VENDOR_INVENSENSE,       /**< InvenSense */
	SENSOR_VENDOR_KIONIX,           /**< Kionix, Inc. (ROHM) */
	SENSOR_VENDOR_OSRAM,            /**< OSRAM Opto Semiconductors */
	SENSOR_VENDOR_STM,              /**< STMicroelectronics */
	SENSOR_VENDOR_TAOS,             /**< Texas Advanced Optoelectronic
	                                 * Solutions */
	SENSOR_VENDOR_FREESCALE,        /**< Freescale Semiconductor */
	SENSOR_VENDOR_TI				/**< Texas Instruments */
} sensor_vendor_t;

/** \brief Sensor Data Descriptor */
typedef struct {
	union {
		int32_t value [3];          /**< Generic data name */

		struct {                    /* Linear axis data */
			int32_t x;              /**< x-axis value */
			int32_t y;              /**< y-axis value */
			int32_t z;              /**< z-axis value */
		} axis;

		struct {                    /* Rotational axis data */
			int32_t pitch;          /**< Pitch-axis value */
			int32_t roll;           /**< Roll-axis value */
			int32_t yaw;            /**< Yaw-axis value */
		} angle;

		struct {                    /* Magnetic heading data */
			int32_t direction;      /**< Heading relative to magnetic north */
			int32_t inclination;    /**< Inclination above/below horizontal */
			int32_t strength;       /**< Net magnetic field intensity */
		} heading;

		struct {                    /* Tap detection data */
			int32_t count;          /**< Tap count */
			int32_t axis;           /**< Tap detection axis (X,Y,Z) */
			int32_t direction;      /**< Tap direction */
		} tap;

		struct {                    /* Pressure data */
			int32_t value;          /**< Pressure value */
			int32_t unused[2];      /**< Unused (spare) fields */
		} pressure;

		struct {                    /* Temperature data */
			int16_t value;          /**< Temperature value */
			int16_t unused[5];      /**< Unused (spare) fields */
		} temperature;

		struct {                    /* Light data */
			int32_t value;          /**< Light value */
			int32_t unused[2];      /**< Unused (spare) fields */
		} light;

		struct {                    /* Proximity data */
			int32_t value[3];       /**< Proximity values (3) */
		} proximity;

		struct {                    /* Voltage data */
			int16_t value;          /**< Temperature value */
			int16_t unused[5];      /**< Unused (spare) fields */
		} voltage;

		struct {
			uint32_t id;            /**< Device ID */
			uint32_t version;       /**< Version */
			uint32_t unused;        /**< Unused (spare) field */
		} device;

		struct {
			struct {
				uint8_t x;
				uint8_t y;
				uint8_t z;
			}axis;
			uint8_t reserved;
			int16_t unused[4];       /**< Unused (spare) field */
		} acc;
	};

	uint32_t timestamp;             /**< Data sample timestamp */
	bool scaled;                    /**< Data sample format (true => scaled) */
} sensor_data_t;

/** \brief Sensor Data Read Operations */
typedef enum {
	SENSOR_READ_ACCELERATION,       /**< Read acceleration data */
	SENSOR_READ_FIELD,              /**< Read field strength data */
	SENSOR_READ_HEADING,            /**< Read heading data */
	SENSOR_READ_ID,                 /**< Read sensor device ID */
	SENSOR_READ_INTENSITY,          /**< Read intensity data */
	SENSOR_READ_LIGHT,              /**< Read light level data */
	SENSOR_READ_PRESSURE,           /**< Read pressure data */
	SENSOR_READ_PROXIMITY,          /**< Read proximity data */
	SENSOR_READ_ROTATION,           /**< Read rotation data */
	SENSOR_READ_TEMPERATURE,        /**< Read temperature data */
	SENSOR_READ_DIE_TEMPERATURE,	/**< Read temperature data */
	SENSOR_READ_OBJ_TEMPERATURE,	/**< Read temperature data */
	SENSOR_READ_VOLTAGE				/**< Read voltage data */
} sensor_read_t;

/** \brief Sensor Command Constants - used for sensor ioctl operations */
typedef enum {
	/* Get operational states */
	SENSOR_INFO_QUERY,              /**< Sensor information query */
	SENSOR_STATUS_RUNNING,          /**< Test for a running sensor */
	SENSOR_STATUS_SLEEPING,         /**< Test for a sleeping sensor */
	SENSOR_STATUS_SUSPENDED,        /**< Test for a suspended sensor */

	/* Set operational states */
	SENSOR_EXEC_SLEEP,              /**< Execute sensor sleep state */
	SENSOR_EXEC_WAKE,               /**< Execute sensor wake state */
	SENSOR_EXEC_SUSPEND,            /**< Execute sensor suspend state */
	SENSOR_EXEC_RESET,              /**< Execute sensor soft-reset function */
	SENSOR_EXEC_SELFTEST,           /**< Execute sensor self-test function */

	/* Get operational parameters */
	SENSOR_GET_RANGE,               /**< Get device-specific operational
	                                 * range */
	SENSOR_GET_BANDWIDTH,           /**< Get bandwidth (Hertz) */
	SENSOR_GET_POWER,               /**< Get power requirement (milliamps) */
	SENSOR_GET_RESOLUTION,          /**< Get sample resolution (bits) */
	SENSOR_GET_SAMPLE_RATE,         /**< Get device-specific sample rate */
	SENSOR_GET_THRESHOLD,           /**< Get sensor threshold value */
	SENSOR_GET_SLEEP_TIME,          /**< Get sleep time (milliseconds) */

	/* Set operational parameters */
	SENSOR_SET_STATE,               /**< Set device operational state/mode */
	SENSOR_SET_RANGE,               /**< set device-specific operational
	                                 * range */
	SENSOR_SET_BANDWIDTH,           /**< Set bandwidth (Hertz) */
	SENSOR_SET_POWER,               /**< Set power requirement (milliamps) */
	SENSOR_SET_CURRENT,             /**< Set current level (milliamps) */
	SENSOR_SET_RESOLUTION,          /**< Set sample resolution (bits) */
	SENSOR_SET_SAMPLE_RATE,         /**< Set device-specific sample rate */
	SENSOR_SET_THRESHOLD,           /**< Set sensor threshold value */
	SENSOR_SET_SLEEP_TIME,          /**< Set sleep time (milliseconds) */
	SENSOR_SET_TAP,                 /**< Set sensor tap detection values */

	/* Sensor event control */
	SENSOR_ENABLE_EVENT,            /**< Enable sensor event */
	SENSOR_DISABLE_EVENT,           /**< Disable sensor event */

	/* Sensor data acquisition */
	SENSOR_READ_SCALAR,             /**< Read generic single-value data */
	SENSOR_READ_VECTOR              /**< Read generic 3-axis vector data */
} sensor_command_t;

/** \brief Sensor Driver and API Error Constants */
typedef enum {
	SENSOR_ERR_NONE             = 0x0000,
	SENSOR_ERR_CALDATA          = 0x0001,
	SENSOR_ERR_RAWDATA          = 0x0002,
	SENSOR_ERR_COMPDATA         = 0x0004,
	SENSOR_ERR_HARDWARE         = 0x0008,
	SENSOR_ERR_SOFTWARE         = 0x0010,
	SENSOR_ERR_PARAMS           = 0x0020,
	SENSOR_ERR_CONFIG           = 0x0040,
	SENSOR_ERR_SELFTEST         = 0x0080,
	SENSOR_ERR_IO               = 0x0100,
	SENSOR_ERR_DRIVER           = 0x0200,
	SENSOR_ERR_UNSUPPORTED      = 0x8000,
	SENSOR_ERR_FUNCTION         = 0X8200,
	SENSOR_ERR_IOCTL            = 0x8210
} sensor_error_t;

/** \brief Sensor Self-Test Error Constants */
typedef enum {
	SENSOR_TEST_ERR_NONE        = 0x0000,
	SENSOR_TEST_ERR_FUNCTION    = 0x0001,  /**< No such self-test function */
	SENSOR_TEST_ERR_READ        = 0x0002,  /**< Read error */
	SENSOR_TEST_ERR_WRITE       = 0x0003,  /**< Write error */
	SENSOR_TEST_ERR_RANGE       = 0x0004   /**< Results out of range */
} sensor_testerr_t;

/** \brief Sensor Channel Constants - used to select channel within device */
#define SENSOR_CHANNEL_ALL          (-1)   /**< Select all channels in device */

/** \name Sensor Physical Axis Definitions */

/** \brief Sensor Axis Name Constants */
typedef enum {
	SENSOR_AXIS_X               =  0,      /* Valid indices are 0, 1, 2 */
	SENSOR_AXIS_Y               =  1,
	SENSOR_AXIS_Z               =  2,
	SENSOR_AXIS_NONE            = -1
} sensor_axis_t;

/** \brief Sensor Axis Sign Constants */
typedef enum {
	SENSOR_SIGN_POS             =  1,
	SENSOR_SIGN_NEG             = -1,
	SENSOR_SIGN_NONE            =  0
} sensor_sign_t;

/** \brief Sensor Axis Descriptor */
typedef struct {
	sensor_axis_t axis;
	sensor_sign_t sign;
} sensor_axis_vec_t;

/** \brief Sensor Orientation Descriptor */
typedef struct {
	sensor_axis_vec_t x;
	sensor_axis_vec_t y;
	sensor_axis_vec_t z;
} sensor_orient_t;

#define AXIS_X_POS  {SENSOR_AXIS_X, SENSOR_SIGN_POS}
#define AXIS_X_NEG  {SENSOR_AXIS_X, SENSOR_SIGN_NEG}
#define AXIS_Y_POS  {SENSOR_AXIS_Y, SENSOR_SIGN_POS}
#define AXIS_Y_NEG  {SENSOR_AXIS_Y, SENSOR_SIGN_NEG}
#define AXIS_Z_POS  {SENSOR_AXIS_Z, SENSOR_SIGN_POS}
#define AXIS_Z_NEG  {SENSOR_AXIS_Z, SENSOR_SIGN_NEG}
#define AXIS_NONE   {SENSOR_AXIS_NONE, SENSOR_SIGN_NONE}

/** \brief Sensor Calibration Type Constants */
typedef enum {
	AUTO_CALIBRATE,            /**< Auto-calibration */
	MANUAL_CALIBRATE,          /**< Manual calibration */
	HARD_IRON_CALIBRATE,       /**< Hard-iron calibration */
	SOFT_IRON_CALIBRATE,       /**< Soft-iron calibration */
	FIELD_CALIBRATE,           /**< Field calibration */
	PROXIMITY_CALIBRATE        /**< Proximity calibration */
} sensor_calibration_t;

/** \brief Sensor Proximity Indicator Constants */
typedef enum {
	PROXIMITY_NONE,            /**< No proximity detected */
	PROXIMITY_FAR,             /**< Far proximity detected */
	PROXIMITY_MEDIUM,          /**< Medium proximity detected */
	PROXIMITY_NEAR             /**< Near proximity detected (default if 1
	                            * level) */
} sensor_proximity_t;

/** \name Sensor Event Interface */

/** \brief Sensor Event Types */
typedef enum {
	SENSOR_EVENT_UNKNOWN        = 0x0000,     /**< Unknown sensor event */
	SENSOR_EVENT_NEW_DATA       = 0x0001,     /**< New data available */
	SENSOR_EVENT_MOTION         = 0x0002,     /**< Motion detected */
	SENSOR_EVENT_LOW_G          = 0x0004,     /**< Inertial low-G detected */
	SENSOR_EVENT_HIGH_G         = 0x0008,     /**< Inertial high-G detected */
	SENSOR_EVENT_TILT           = 0x0010,     /**< Inertial tilt detected */
	SENSOR_EVENT_TAP            = 0x0020,     /**< Tap detected */
	SENSOR_EVENT_S_TAP          = 0x0020,     /**< Single tap detected */
	SENSOR_EVENT_D_TAP          = 0x0040,     /**< Double tap detected */
	SENSOR_EVENT_DROP           = 0x0080,     /**< Drop detected */
	SENSOR_EVENT_NEAR_PROXIMITY = 0x0100,     /**< Near proximity detected */
	SENSOR_EVENT_MED_PROXIMITY  = 0x0200,     /**< Medium proximity detected */
	SENSOR_EVENT_FAR_PROXIMITY  = 0x0400,     /**< Far proximity detected */
	SENSOR_EVENT_LOW_LIGHT      = 0x0800,     /**< Low light level detected */
	SENSOR_EVENT_HIGH_LIGHT     = 0x1000      /**< High light level detected */
} sensor_event_t;

/** \brief Sensor Event Data Descriptor */

typedef struct {
	sensor_event_t event;                     /**< Sensor event type */
	int16_t channel;                          /**< Channel (event source) */
	sensor_data_t data;                       /**< Sensor event data */
} sensor_event_data_t;

/** \brief Sensor Tap Event */
typedef enum {
	SENSOR_TAP_AXIS_UNKNOWN     = 0x0000,     /**< Unknown tap detect axis */
	SENSOR_TAP_AXIS_X           = 0x0001,     /**< Tap detected on X axis */
	SENSOR_TAP_AXIS_Y           = 0x0002,     /**< Tap detected on Y axis */
	SENSOR_TAP_AXIS_Z           = 0x0004,     /**< Tap detected on Z axis */
} sensor_tap_axis_t;

/** \brief Sensor Tap Event (event direction) */
typedef enum {
	SENSOR_TAP_DIRECTION_UNKNOWN = 0x0000,    /**< Unknown tap direction */
	SENSOR_TAP_DIRECTION_POS     = 0x0001,    /**< Tap in positive direction */
	SENSOR_TAP_DIRECTION_NEG     = 0x0002,    /**< Tap in negative direction */
} sensor_tap_direction_t;

/** \brief Sensor Tap Event Parameters */
typedef struct {
	size_t count;                   /**< Max number of taps to detect */
	uint16_t axes;                  /**< Axes (X,Y,Z) to detect tap events */
	size_t threshold_min;           /**< Minimum signal change for tap */
	size_t threshold_max;           /**< Maximum signal change for tap */
	uint16_t total_time;            /**< Total time for tap detection window */
	uint16_t tap_time_min;          /**< Minimum duration of a tap */
	uint16_t tap_time_max;          /**< Maximum duration of a tap */
	uint16_t between_time;          /**< Minimum time between taps */
	uint16_t ignore_time;           /**< Latency period after a tap */
} sensor_tap_params_t;

/** \brief Sensor Operational State Type Constants */
typedef enum {
	SENSOR_STATE_UNKNOWN,           /**< Unknown state */
	SENSOR_STATE_NORMAL,            /**< Normal state */
	SENSOR_STATE_SLEEP,             /**< Sleep state */
	SENSOR_STATE_SUSPEND,           /**< Suspend state */
	SENSOR_STATE_LOWEST_POWER,      /**< Lowest power state */
	SENSOR_STATE_LOW_POWER,         /**< Low-power state */
	SENSOR_STATE_HIGH_POWER,        /**< High-power state */
	SENSOR_STATE_HIGHEST_POWER,     /**< Highest power state */
	SENSOR_STATE_X_AXIS_STANDBY,    /**< x-axis low power standby */
	SENSOR_STATE_Y_AXIS_STANDBY,    /**< y-axis low power standby */
	SENSOR_STATE_Z_AXIS_STANDBY,    /**< z-axis low power standby */
	SENSOR_STATE_RESET,             /**< Reset state (run soft-reset) */
	SENSOR_STATE_POWER_DOWN,        /**< Power-down state */
	SENSOR_STATE_SINGLE,            /**< Single measurement mode */
	SENSOR_STATE_CONTINUOUS         /**< Continuous measurement mode */
} sensor_state_t;

/** \brief Sensor Self Test Types */
typedef enum {
	SENSOR_TEST_DEFAULT,            /**< Default test type for device */
	SENSOR_TEST_BIAS_POS,           /**< Positive bias test */
	SENSOR_TEST_BIAS_NEG,           /**< Negative bias test */
	SENSOR_TEST_DEFLECTION,         /**< Deflection test (e.g. accelerometer) */
	SENSOR_TEST_INTERRUPT           /**< Interrupt test */
} sensor_test_t;

/** \brief Sensor Threshold Type Constants */
typedef enum {
	SENSOR_THRESHOLD_MOTION,         /**< Inertial motion (slope) threshold */
	SENSOR_THRESHOLD_LOW_G,          /**< Inertial low-G threshold */
	SENSOR_THRESHOLD_HIGH_G,         /**< Inertial high-G threshold */
	SENSOR_THRESHOLD_TAP,            /**< Inertial tap threshold */
	SENSOR_THRESHOLD_S_TAP,          /**< Inertial single tap threshold */
	SENSOR_THRESHOLD_D_TAP,          /**< Inertial double tap threshold */
	SENSOR_THRESHOLD_TILT,           /**< Inertial tilt threshold */
	SENSOR_THRESHOLD_DURATION,       /**< Event duration */
	SENSOR_THRESHOLD_LOW_LIGHT,      /**< Low light level threshold */
	SENSOR_THRESHOLD_HIGH_LIGHT,     /**< High light level threshold */
	SENSOR_THRESHOLD_NEAR_PROXIMITY, /**< Near proximity threshold */
	SENSOR_THRESHOLD_MED_PROXIMITY,  /**< Medium proximity threshold */
	SENSOR_THRESHOLD_FAR_PROXIMITY,  /**< Far proximity threshold */
} sensor_threshold_t;

/** \brief Sensor Threshold Descriptor */
typedef struct {
	sensor_threshold_t type;        /**< Threshold type */
	int16_t value;                  /**< Threshold value */
} sensor_threshold_desc_t;

/** ! \name Sensor Sample Commands and Data Formats */

/** \brief Sensor Sample Data Format */
typedef enum {
	SAMPLE_FORMAT_COUNTS,           /**< Unscaled raw sample counts */
	SAMPLE_FORMAT_SCALED            /**< Scaled engineering units */
} sample_format_t;

/** \brief Sensor Sample Units */
typedef enum {
	SENSOR_UNITS_NONE,              /**< Unscaled data (raw counts) */
	SENSOR_UNITS_g0,                /**< Standard gravity (g0) */
	SENSOR_UNITS_gauss,             /**< Gauss (G) */
	SENSOR_UNITS_lux,               /**< Lux (lx) */
	SENSOR_UNITS_lumen,             /**< Lumen (lm) */
	SENSOR_UNITS_pascal,            /**< Pascal (Pa) */
	SENSOR_UNITS_deg_per_sec,       /**< Degrees-per-second (dps) */
	SENSOR_UNITS_deg_Celcius,       /**< Degrees-Celcius */
	SENSOR_UNITS_tesla,             /**< Tesla (T) */
	SENSOR_UNITS_volt_DC,           /**< Volts DC */
	SENSOR_UNITS_volt_AC            /**< Volts AC */
} sensor_units_t;

/** \brief Sensor Sample Unit Scale */
typedef enum {
	SENSOR_SCALE_micro  = -6,       /**< Log 0.000001 */
	SENSOR_SCALE_milli  = -3,       /**< Log 0.001 */
	SENSOR_SCALE_centi  = -2,       /**< Log 0.01 */
	SENSOR_SCALE_deci   = -1,       /**< Log 0.1 */
	SENSOR_SCALE_one    =  0,       /**< Log 1 */
	SENSOR_SCALE_deca   =  1,       /**< Log 10 */
	SENSOR_SCALE_hecto  =  2,       /**< Log 100 */
	SENSOR_SCALE_kilo   =  3        /**< Log 1000 */
} sensor_scale_t;

/** \brief Sensor Operational Capabilities Descriptors */

typedef struct {
	union {
		int16_t bandwidth_Hz;       /**< Engineering unit value (Hz.) */
		int16_t range_units;        /**< Engineering unit value (varies) */
	};
	uint8_t reserved_val;           /**< Driver-specific reserved value */
} sensor_map_t;

typedef sensor_map_t sensor_range_t;
typedef sensor_map_t sensor_band_t;

/** \brief Sensor Capabilities */
typedef struct {
	sensor_feature_t feature;        /**< API-specific sensor features */
	sensor_vendor_t vendor;          /**< API-specific vendor designation */
	size_t range_count;              /**< Device-specific range count */
	size_t band_count;               /**< Device-specific bandwidth count */
	const sensor_map_t *range_table; /**< Device-specific range table */
	const sensor_map_t *band_table;  /**< Device-specific bandwidth table */
	sensor_units_t units;            /**< Data sample base engineering units */
	sensor_scale_t scale;            /**< Data sample engineering unit scale */
	const char *name;                /**< Human readable description */
} sensor_caps_t;

/** ! \name Sensor Device Descriptors */

/** \brief Sensor Platform Hardware Abstraction Descriptor */
typedef struct {
	uint8_t burst_addr;              /**< Sensor Burst Read Address */
	uint32_t mcu_sigint;             /**< I/O input to MCU from sensor */
	uint32_t mcu_sigout;             /**< I/O output to MCU from sensor */
	sensor_type_t dev_type;          /**< Sensor Device Type */

	/* Sensor Physical/Logical Orientation */
	sensor_orient_t orientation;     /**< Sensor axis/sign used as X,Y,Z */

	/* Sensors Temperature value */


	/* Sensor Operational Parameters */
	int16_t range;                   /**< Sensor range (engineering units) */
	int16_t bandwidth;               /**< Sensor bandwidth (Hz) */
	int16_t sample_rate;             /**< Sensor sample rate (Hz) */
	int16_t resolution;              /**< Sensor sample resolution (bits) */
}sensor_hal_t;

/** \interface sensor_event_callback
 *
 * \brief Sensor Event Handler Callback Type
 */
typedef bool (*sensor_event_callback)(void* sensor, void *arg);

/** \interface sensor_enum_callback
 *
 * \brief Sensor Enumeration Handler Callback Type
 */
typedef bool (*sensor_enum_callback)(void* sensor, void *arg);

/** \brief allocate one per event type (v. one per event) */
typedef struct exec_list {
	sensor_event_callback handler;          /**< Sensor event callback */
	void *arg;                     			/**< Event callback argument */
	bool enable;							/**< Enabled */
	sensor_event_t event;
	struct exec_list* leaf;					/**< The next leaf */
} sensor_event_callback_t;

/** \brief Create sneosrint handler */
typedef void (*sensor_int_handler)();

/** ! \name Sensor API Convenience Macros and Functions */
#define ARRAYSIZE(a)                    (sizeof(a) / sizeof(a[0]))

/*!
 * \name Sensor Base I/O Access Methods
 */

/**
 * \brief The Sensor Interface
 *
 *  This it the sensor driver interface class that incorporates all necessary
 * 	bus structures and needed definitions.
 */
class sensor {

	/*
	 * Protected sensor attributes
	 */
	protected:

		/*
		 * Execution handlers
		 */
		sensor_event_callback_t	handlers;		/**< Sensor handlers */

		/*
		 * Bus Handle
		 */
		bus_t*					bus;			/**< Bus Handle */

	/*
	 * Public sensor attributes
	 */
	public:

		/*
		 * Virtual cache
		 */
		void* 			cache;

		/*
		 * Cache type
		 */
		uint16_t 		cache_type;

		/*
		 * Message type
		 */
		msg_type_t				msg;

		/*
		 * Sensor attributes
		 */
		sensor_caps_t 			caps;			/**< Sensor capabilitites */
		sensor_hal_t			hal;			/**< Sensor hardware abstrction handle */
		sensor_type_t 			type;           /**< Sensor type (operational mode) */
		sensor_state_t 			mod;            /**< Runtime state */
		sensor_error_t 			err;            /**< Runtime errors */
		int16_t 				channel;        /**< Channel number within sensor */
		void 					*aux;           /**< API extensions */

		/**
		 * \brief Updates the internal cache of the sensor.
		 *
		 * In this function, the system reads the newest data from the sensor and
		 * updates the internal caches that are mapped to the internal system BIOS.
		 * We do this to obtain a transparency between the BIOS and the sensor.
		 *
		 * @return	bool 	true of the update was successful.
		 */
		virtual bool update();

		/**
		 * Returns object address.
		 */
		void* get_address(){
			return this;
		}


	/*
	 * Protected sensor methods
	 */
	protected:

		/*!
		 *\brief The default constructor for the class.
		 *
		 * Nothing is done in the constructor as this class is mearly an
		 * interface to the sensor class.
		 */
		sensor(){};

		/*!
		 *\brief The default deconstructor for the class.
		 *
		 * Nothing is done in the constructor as this class is mearly an
		 * interface to the sensor class.
		 */
		~sensor(){};

		/*
		 * Events
		 */

		/**
		 * @brief The Interrupt Attach Mechanism
		 *
		 * This Routine attaches an intterupt to a specified pin with
		 * the specified mode. The handler passed to the method will
		 * be registered as being the interrupt handler.
		 *
		 * @param pin		The pin to attach the interrupt on
		 * @param mode		The mode of the interrupt
		 * @param handler	The handler function
		 */
		void irq_connect(uint32_t pin, uint8_t mode, sensor_int_handler handler);

		/**
		 * \brief Install a sensor event handler.
		 *
		 * This routine installs a sensor event handler that will be called when
		 * the specified sensor events occur. Function parameters specify the
		 * specific sensor event to be reported, the address of the handler
		 * function to be called, an optional parameter that will be passed to the
		 * handler, and whether the event handler should initially be enabled or
		 * disabled.
		 *
		 * The event handler can subsequently be enabled and disabled using the
		 * sensor_event_enable() and sensor_event_disable() routines.
		 *
		 * \param  sensor_event Specifies the sensor event type
		 * \param  handler      Specifies an application-defined callback
		 * \param  arg          Specifies an optional callback argument
		 * \param  enable       Specifies whether or not the event should be enabled
		 * \return bool     true if the call succeeds, else false is returned.
		 */
		bool event(sensor_event_t sensor_event, sensor_event_callback handler,
								void *arg, bool enable);

		/**
		 * \brief This function returns the current timestamp counter value.
		 *
		 * The timestamp facility is implemented in terms of the XMEGA or UC3
		 * timer and clock function APIs.
		 *
		 * \return The current counter value (microseconds).
		 */
		uint32_t timestamp(void);

		/*
		 * Virtual Methods
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
		virtual bool get_device_id();

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
		virtual bool read(sensor_read_t type);

		/**
		 * \brief Calibrate a sensor device.
		 *
		 * \param   caltype The type of calibration to perform.
		 * \param   code    Device-specific calibration code or step parameter.
		 * \param   info    Unimplemented (ignored) parameter.
		 * \return  bool    true if the call succeeds, else false is returned.
		 */
		virtual bool calibrate(sensor_calibration_t caltype, int code, void *info);

		/**
		 * \brief Initiate a sensor device software reset.
		 *
		 * \param   arg     Device-specific argument options.
		 * \return  bool    true if the call succeeds, else false is returned.
		 */
		virtual bool reset(int arg);

		/**
		 * \brief Set a sensor device to low-power or standby mode.
		 *
		 * \param   arg     Device-specific argument options.
		 * \return  bool    true if the call succeeds, else false is returned.
		 */
		virtual bool sleep(int arg);

		/**
		 * \brief Set a sensor operational threshold.
		 *
		 * \param   threshold   A specified sensor operational threshold.
		 * \param   value       The value of the specified threshold.
		 * \return  bool     true if the call succeeds, else false is returned.
		 */
		virtual bool set_threshold(sensor_threshold_t threshold, int16_t value);

		/**
		 * \brief Get a sensor operational threshold.
		 *
		 * \param   sensor      The address of an initialized sensor descriptor.
		 * \param   threshold   A specified sensor operational threshold.
		 * \param   value       Address of location to return threshold value
		 *
		 * \return  bool     true if the call succeeds, else false is returned.
		 */
		virtual bool get_threshold(sensor_threshold_t threshold, int16_t *value);

		/**
		 * \brief Execute a sensor device control function.
		 *
		 * \param   cmd     Specifies the IOCTL command.
		 * \param   arg     Specifies command parameters (varies by command).
		 * \return  bool    true if the call succeeds, else false is returned.
		 */
		virtual bool ioctl(sensor_command_t cmd, void *arg);

		/**
		 * \brief Activate a sensor self-test function.
		 *
		 * \param test_code Address of a device-specific numeric test code.
		 * \param arg       Device-specific self-test argument options.
		 * \return bool     true if the test succeeds, else false is returned.
		 */
		virtual bool selftest(int *test_code, void *arg);

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
		virtual bool set_state(sensor_state_t mode);

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
		virtual bool get_state(sensor_state_t *mode);

		/**
		 * \brief Enable a sensor event handler.
		 *
		 * This routine enables the handler for a sensor event.  The event handler
		 * must have already been defined using sensor_add_event().  To disable
		 * the event, see sensor_disable_event().
		 *
		 * \param  sensor		The sensor object
		 * \param  sensor_event Specifies the sensor event type
		 * \return bool         true if the call succeeds, else false is returned.
		 */
		static inline bool enable_event(sensor* sensor, sensor_event_t sensor_event){
			return sensor->ioctl(SENSOR_ENABLE_EVENT, &sensor_event);
		}

		/**
		 * \brief Disable a sensor event handler.
		 *
		 * This routine disables the handler for a sensor event.  The event handler
		 * must have already been defined using sensor_add_event().  To enable
		 * the event, see sensor_enable_event().
		 *
		 * \param  sensor		The sensor object
		 * \param  sensor_event Specifies the sensor event type
		 * \return bool         true if the call succeeds, else false is returned.
		 */
		static inline bool disable_event(sensor* sensor, sensor_event_t sensor_event) {
			return sensor->ioctl(SENSOR_DISABLE_EVENT, &sensor_event);
		}

	/*
	 * Public static utilities
	 */
	public:

		/**
		 * @brief Register a new handler for execution in interrupt
		 *
		 * @param handler	The handler to register
		 * @return nothing
		 */
		bool register_handler(sensor_event_callback_t* handler);

		/**
		 * \brief Convert raw sensor data to scaled engineering units.
		 *
		 * This routine converts a sensor sample, \c counts, to a linearly scaled
		 * integer value in engineering units using device-specific range in
		 * standard engineering units and full scale resolution parameters in
		 * \c hal.
		 *
		 * \param hal       An initialized hardware interface descriptor.
		 * \param counts    An unscaled raw sensor sample value.
		 *
		 * \return Scaled signed sensor data in standard engineering units.
		 */
		static inline int32_t raw_to_scaled(sensor* sensor, int32_t counts){

			/* The unit increment per count is peak-to-peak range divided
			 * by full-scale resolution.
			 */
			return (counts * (2 * (int32_t)(sensor->hal.range))) >> sensor->hal.resolution;
		}

		/**
		 * \brief Convert scaled sensor data to raw counts
		 *
		 * This routine converts a linearly scaled integer value in engineering
		 * units to the corresponding "raw" reading value for the device,
		 * using the device-specific range in standard engineering units and full
		 * scale resolution parameters in "opts".
		 *
		 * \param hal       An initialized hardware interface descriptor.
		 * \param value     A scaled sensor sample value.
		 *
		 * \return Scaled signed sensor data in standard engineering units.
		 */
		static inline int32_t scaled_to_raw(sensor* sensor, int32_t value){

			/* The unit increment per count is peak-to-peak range divided
			 * by full-scale resolution.
			 */
			return (value << sensor->hal.resolution) / (2 * (int32_t)(sensor->hal.range));
		}

		/**
		 * @brief Default sensor event callback handler
		 *
		 * The default event callback handler returns immediately.
		 *
		 * @param data 		Sensor event data descriptor
		 * @param arg  		Optional user-specified callback argument
		 * @return nothing
		 */
		static inline bool default_event_handler(void* data, void* arg){
			return true;
		}
};

typedef sensor sensor_t;

#endif /* PLATFORM_SENSOR_SENSOR_SENSOR_H_ */

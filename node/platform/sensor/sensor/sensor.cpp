/*
 * sensor.cpp
 *
 *  Created on: Jul 6, 2015
 *      Author: francis-ccs
 */

#include <platform/sensor/sensor/sensor.h>

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
void sensor::irq_connect(uint32_t pin, uint8_t mode, sensor_int_handler handler){

	/*
	 * Attach an interrupt on the pin with mode specified.
	 */
	attachInterrupt(pin, handler, mode);
	return;
}

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
bool sensor::event(sensor_event_t sensor_event, sensor_event_callback handler,
						void *arg, bool enable){

	/*
	 * Create a callback struct
	 */
	sensor_event_callback_t callback;

	/*
	 * Set the callback
	 */
	if(handler){
		callback.handler = handler;
	}else{
		callback.handler = sensor::default_event_handler;
	}

	/*
	 * Set the arg
	 */
	callback.arg 	 	= arg;
	callback.enable 	= enable;
	callback.event		= sensor_event;

	/*
	 * Register the event handler
	 */
	return register_handler(&callback);
}

/**
 * \brief This function returns the current timestamp counter value.
 *
 * The timestamp facility is implemented in terms of the XMEGA or UC3
 * timer and clock function APIs.
 *
 * \return The current counter value (microseconds).
 */
uint32_t sensor::timestamp(void){

	/*
	 * return the millis past since bootup
	 */
	return millis();
}

/**
 * \brief Updates the internal cache of the sensor.
 *
 * In this function, the system reads the newest data from the sensor and
 * updates the internal caches that are mapped to the internal system BIOS.
 * We do this to obtain a transparency between the BIOS and the sensor.
 *
 * @param	sensor	Sensor class object pointer
 * @return	bool 	true of the update was successful.
 */
bool sensor::update(sensor_t* sensor){

	/*
	 * Update the sensor
	 */
	return sensor->run();
}


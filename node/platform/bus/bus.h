/*
 * bus.h
 *
 *  Created on: Jul 4, 2015
 *      Author: francis-ccs
 */

#ifndef SENSORS_BUS_H_
#define SENSORS_BUS_H_

#include <Arduino.h>
#include <status_codes.h>

/*!
 * \brief Supported Bus Types
 */
typedef enum {
	BUS_TYPE_UNKNOWN,					//!< BUS_TYPE_UNKNOWN
	BUS_TYPE_I2C,    					//!< BUS_TYPE_I2C
	BUS_TYPE_SPI,   					//!< BUS_TYPE_SPI
	BUS_TYPE_PMBUS,  					//!< BUS_TYPE_PMBUS
	BUS_TYPE_SMBUS, 					//!< BUS_TYPE_SMBUS
}bus_type_t;

/*!
 * \brief Sensor Status Codes
 */
typedef status_code_t bus_status_t;

/*!
 * \brief Sensor Packet Definition
 */
typedef void* bus_packet_t;

/*!
 * \name System Bus I/O Access Methods
 */

/**
 * \brief The Bus Interface
 *
 * 	This it the bus driver interface class that incorporates all necessary
 * 	bus structures and needed definitions.
 */
class bus {

	/**
	 * Public class attributes
	 */
	protected:

		bus_type_t 		type;               /**< Bus type and protocol */
		bus_status_t 	status;             /**< Bus transaction status */
		bool 			no_wait;          	/**< Bus transaction non-wait option */

	/**
	 * Public class methods
	 */
	public:

		/*!
		 * \internal Initialize the bus I/O interface.
		 */
		bus(){}

		/*!
		 * \brief The Default Deconstructor for the Object
		 */
		~bus(){}

		/**
		 * @brief Public accessor for the bus status
		 *
		 * Accesses the bus status
		 */
		virtual bus_status_t get_status();

		/*!
		 * \brief Read multiple Bytes from a bus interface.
		 *
		 * \param   addr    The device register or memory address.
		 * \param   data    The destination read buffer address.
		 *
		 * \return The number of Bytes read, which may be less than the
		 *         requested number of Bytes in the event of an error.
		 */
		virtual size_t read(uint8_t addr, bus_packet_t data);
		virtual size_t read(uint8_t addr, uint8_t size, uint8_t index, uint8_t* data);

		/*!
		 * \brief Write multiple Bytes to a bus interface.
		 *
		 * \param   addr    The device register or memory address.
		 * \param   data    The source write buffer address.
		 *
		 * \return The number of Bytes written, which may be less than the
		 *         requested number of Bytes in the event of an error.
		 */
		virtual size_t write(uint8_t addr, bus_packet_t data);
		virtual size_t write(uint8_t addr, uint8_t size, uint8_t index, uint8_t* data);

		/*!
		 * \brief Determine the existence of a bus device
		 *
		 * This routine determines the existence of a device located at a bus interface
		 * and address specified by an initialized \c bus descriptor.
		 * Implementations are only required to return \c true when it can be determined
		 * that a device is installed at the bus interface address.
		 *
		 * \retval  true    A device responded to the bus address.
		 * \retval  false   A device did not respond to the bus address.
		 */
		virtual bool probe();

		/*!
		 * \brief Read a field stored at a device register or memory address
		 *
		 * This routine reads a specified value from a bit field within a 1-Byte
		 * device register or memory address. The set bits in the mask parameter
		 * determine the field location. For example, if the mask is 30h and the
		 * value AFh is stored in the register, the value 2h will be returned.
		 *
		 * \param   bus     An initialized bus interface descriptor.
		 * \param   addr    The device register or memory address.
		 * \param   mask    The mask of the field to set.
		 *
		 * \return  The value stored in the register or memory field.
		 */
		virtual uint8_t reg_fieldget(uint8_t addr, uint8_t mask);

		/*!
		 * \brief Write a field stored at a device register or memory address
		 *
		 * This routine writes a specified value to a bit field within a 1-Byte
		 * device register or memory address. The set bits in the mask parameter
		 * determine the field location. For example, if the mask is 30h and the
		 * value is 2h, the value 20h will be bitwise logically OR'd into the
		 * 1-Byte register value after clearing the bit values in the field.
		 *
		 * \param   bus     An initialized bus interface descriptor.
		 * \param   addr    The device register or memory address.
		 * \param   mask    The mask of the field to set.
		 * \param   value   The value of the field to set.
		 *
		 * \return  Nothing
		 */
		virtual void reg_fieldset(uint8_t addr, uint8_t mask, uint8_t value);

		/*!
		 * \brief Read a single Byte from a bus interface.
		 *
		 * \param   addr    The device register or memory address.
		 * \param	index	The memory index to get
		 *
		 * \return A value fetched from the device.  This value is
		 *         undefined in the event of an I/O error.
		 */
		virtual uint8_t get(uint8_t addr, uint8_t index);

		/*!
		 * \brief Write a single Byte to a bus interface.
		 *
		 * \param   addr    The device register or memory address.
		 * \param	index	The memory index to get
		 * \param   data    The value of the Byte to write.
		 *
		 * \return  Nothing
		 */
		virtual void put(uint8_t addr, uint8_t index, uint8_t data);

		/*!
		 * \brief Clear a bit at a bus device register or memory address.
		 *
		 * \param   bus     An initialized bus interface descriptor.
		 * \param   addr    The device register or memory address.
		 * \param   mask    The mask value of the bit to clear.
		 * \param	index	The memory index to get
		 *
		 * \return  Nothing
		 */
		inline void reg_bitclear(uint8_t addr, uint8_t index, uint8_t mask)
		{
			put(addr, index, ~mask & get(addr, index));
		}

		/*!
		 * \brief Set a bit at a bus device register or memory address.
		 *
		 * \param   bus     An initialized bus interface descriptor.
		 * \param   addr    The device register or memory address.
		 * \param   mask    The mask value of the bit to set.
		 * \param	index	The memory index to get
		 *
		 * \return  Nothing
		 */
		inline void reg_bitset(uint8_t addr, uint8_t index, uint8_t mask)
		{
			put(addr, index, mask | get(addr, index));
		}

};

/*!
 * \brief Type Definition
 */
typedef bus bus_t;

#endif /* SENSORS_BUS_H_ */

/*
 * bus_i2c.h
 *
 *  Created on: Jul 4, 2015
 *      Author: francis-ccs
 */

#ifndef BUS_I2C_BUS_I2C_H_
#define BUS_I2C_BUS_I2C_H_

#include <Wire.h>
#include <stdint.h>
#include <platform/bus/bus.h>

/**
 * @brief Define an i2c byte
 */
#define I2C_BUS_BYTE		(sizeof(uint8_t))

/**
 * @brief Define I2C Master Address
 */
#define I2C_MASTER_ADDRESS (0x00)

/**
 * @brief I2C Bus Packet Definition
 */
typedef struct {

	struct {
		uint8_t 	mem;		/**< The memory address to read / write **/
		uint8_t* 	data;		/**< The data pointer to write ir read in **/
	}buf;
	uint8_t 		size;		/**< The size to read / write **/
}i2c_bus_packet_t;

/*!
 * \name System Bus I/O Access Methods (i2c)
 */

/**
 * \brief The I2C Bus Interface
 *
 * 	This it the bus driver interface class that incorporates all necessary
 * 	bus structures and needed definitions.
 *
 * 	@extends bus_t
 */
class bus_i2c: public bus {

	/*
	 * State the friend class
	 */
	friend TwoWire;

	/**
	 * Private class attributes
	 */
	private:

		TwoWire*		busif;				/**< Bus interface **/

	/**
	 * Public Class methods
	 */
	public:

		/*!
		 * \brief Initialize the bus I/O interface.
		 */
		bus_i2c();

		/**
		 * @brief Public accessor for the bus status
		 *
		 * Accesses the bus status
		 */
		bus_status_t get_status(){
			return status;
		}

		/*!
		 * \brief Read multiple Bytes from a bus interface.
		 *
		 * \param   addr    The device register or memory address.
		 * \param 	packet	The device packet to read into.
		 *
		 * \return The number of Bytes read, which may be less than the
		 *         requested number of Bytes in the event of an error.
		 */
		size_t read(uint8_t addr, i2c_bus_packet_t* packet);
		size_t read_bytes(int addr, int size, uint8_t index, uint8_t* data);

		/*!
		 * \brief Write multiple Bytes to a bus interface.
		 *
		 * \param   addr    The device register or memory address.
		 * \param 	packet	The device packet to write
		 *
		 * \return The number of Bytes written, which may be less than the
		 *         requested number of Bytes in the event of an error.
		 */
		size_t write(uint8_t addr, i2c_bus_packet_t* packet);
		size_t write_bytes(int addr, unsigned int size, uint8_t index, uint8_t* data);

		/*!
		 * \brief Read a single Byte from a bus interface.
		 *
		 * \param   addr    The device register or memory address.
		 * \param	index	The memory index to get
		 *
		 * \return A value fetched from the device.  This value is
		 *         undefined in the event of an I/O error.
		 */
		uint8_t get(uint8_t addr, uint8_t index);

		/*!
		 * \brief Write a single Byte to a bus interface.
		 *
		 * \param   addr    The device register or memory address.
		 * \param	index	The memory index to get
		 * \param   data    The value of the Byte to write.
		 *
		 * \return  Nothing
		 */
		void put(uint8_t addr, uint8_t index, uint8_t data);

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
		bool probe();

		/*!
		 * \brief Returns the next byte to read.
		 *
		 * This routine uses the buffer to pop the next byte in the receive queue.
		 *
		 * @return uint8_t		The next byte to address.
		 */
		uint8_t peek();

		/*!
		 * \brief Deletes the entries in the queue.
		 *
		 * This routine uses the buffer to flush all entries in the receive queue.
		 */
		void flush();

		/*!
		 * \brief Checks the numebr of bytes in the receive queue.
		 *
		 * This routine uses the number of elements in the recieve queue counter
		 * to return the amount of elements currently in the receive queue.
		 *
		 * @return int		The number of bytes in the receive queue.
		 */
		int available();

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
		 * \param	index	The memory index to get
		 * \return  The value stored in the register or memory field.
		 */
		uint8_t reg_fieldget(uint8_t addr, uint8_t index, uint8_t mask);

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
		 * \param	index	The memory index to get
		 *
		 * \return  Nothing
		 */
		void reg_fieldset(uint8_t addr, uint8_t mask, uint8_t value, uint8_t index);

		/*!
		 * \brief Create a bus pakcet to send/receive.
		 *
		 * This routine creates a packet to be sent or received on the i2c bus.
		 *
		 * @param length	The length of the packet to send/receive
		 * @param addr		The memory address to read
		 * @param data		The data pointer
		 * @param packet	The packet pointer to copy into
		 */
		void packetize(int length, uint8_t addr, uint8_t* data,
				i2c_bus_packet_t* packet);

		/*!
		 * \brief Gets the internal data pointer
		 *
		 * @param  rec		The structure pointer to copy the packet in
		 * @param packet	The packet read from the bus interface
		 * @param  size		The size of the passed structure
		 */
		void get_data(void* rec, i2c_bus_packet_t* packet, size_t size);

		/*!
		 * \brief The Default Deconstructor for the Object
		 */
		~bus_i2c(){}
};

/*!
 * \brief Typedef
 */
typedef bus_i2c bus_i2c_t;

#endif /* BUS_I2C_BUS_I2C_H_ */

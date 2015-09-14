/*
 * bus_i2c.cpp
 *
 *  Created on: Jul 4, 2015
 *      Author: francis-ccs
 */

#include <platform/bus/i2c/bus_i2c.h>

/*!
 * \internal Initialize the bus I/O interface.
 */
bus_i2c::bus_i2c() : bus(){

	/*
	 * Initialize the internal data structures
	 */
	type 		= BUS_TYPE_I2C;
	status		= STATUS_OK;

	/*
	 * We wrap the TwoWire class with our class.
	 */
	busif 		= &Wire;

	/*
	 * Start the I2C Engine
	 */
	busif->begin(I2C_MASTER_ADDRESS);
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
size_t bus_i2c::read(uint8_t addr, i2c_bus_packet_t* packet){

	// Container
	size_t read;

	/*
	 * Address the chip with the address passed (addr)
	 * Read the number of bytes in the queue.
	 */
	busif->beginTransmission(addr);
	busif->write(packet->buf.mem);
	busif->endTransmission();

	// Get the data
	read = busif->requestFrom(addr, packet->size);

	/*
	 * Do a sanity check on the received data
	 */
	if(!(read & busif->available())){

		/*
		 * The number of read bytes does not correspond
		 * to the bytes that are in the queue.
		 *
		 * Return 0 and set the status to error.
		 */
		status = ERR_IO_ERROR;
		return (0);

	}else{

		/*
		 * Create a buffer to hold the data
		 */
		packet->buf.data = (uint8_t*)malloc(packet->size);
		busif->readBytes((char*)packet->buf.data, read);
	}

	/*
	 * We return the number of bytes read (> 0)
	 */
	return (read);
}

size_t bus_i2c::read(int addr, unsigned int  size,
						uint8_t index, uint8_t* data){

	// Container
	i2c_bus_packet_t packet;
	uint8_t read_byte;

	/*
	 * Packetize
	 */
	packetize(size, index, data, &packet);

	/*
	 * Read
	 */
	read_byte = read(addr, &packet);

	/*
	 * Get the data
	 */
	get_data(data, &packet, size);

	/*
	 * Return the size
	 */
	return read_byte;
}

/*!
 * \brief Write multiple Bytes to a bus interface.
 *
 * \param   addr    The device register or memory address.
 * \param 	packet	The device packet to write
 *
 * \return The number of Bytes written, which may be less than the
 *         requested number of Bytes in the event of an error.
 */
size_t bus_i2c::write(uint8_t addr, i2c_bus_packet_t* packet){

	// Container
	size_t written;

	/*
	 * Begin the transmission to a specified chip id and
	 * write the number of bytes provided.
	 */
	busif->beginTransmission(addr);
	written = busif->write((uint8_t*)packet->buf.data, packet->size);

	/*
	 * If there is an error in the i/o interaction
	 */
	if(busif->endTransmission()){
		status = ERR_IO_ERROR;
	}

	/*
	 * Return the size that was written to the bus.
	 */
	return (written);
}

size_t bus_i2c::write(int addr, unsigned int size,
						uint8_t index, uint8_t* data){

	// Container
	i2c_bus_packet_t packet;

	/*
	 * Packetize
	 */
	packetize(size, index, data, &packet);

	/*
	 * Write and return the size written
	 */
	return write(addr, &packet);
}

/*!
 * \brief Read a single Byte from a bus interface.
 *
 * \param   addr    The device register or memory address.
 * \param	index	The memory index to get
 *
 * \return A value fetched from the device.  This value is
 *         undefined in the event of an I/O error.
 */
uint8_t bus_i2c::get(uint8_t addr, uint8_t index){

	/*
	 * Create a packet
	 */
	i2c_bus_packet_t packet;
	packet.size = I2C_BUS_BYTE;
	packet.buf.mem = index;

	/*
	 * Send the packet
	 */
	if(I2C_BUS_BYTE == read(addr, &packet)){

		/*
		 * We have received some data
		 * Return the value at the packet data location
		 */
		return (*packet.buf.data);
	}else{

		/*
		 * The number of read bytes does not correspond
		 * to the bytes that are in the queue.
		 *
		 * Return 0 and set the status to error.
		 */
		status = ERR_IO_ERROR;
		return (0);
	}
}

/*!
 * \brief Write a single Byte to a bus interface.
 *
 * \param   addr    The device register or memory address.
 * \param	index	The memory index to get
 * \param   data    The value of the Byte to write.
 *
 * \return  Nothing
 */
void bus_i2c::put(uint8_t addr, uint8_t index, uint8_t data){

	/*
	 * Create a packet
	 */
	i2c_bus_packet_t packet;
	packet.size = I2C_BUS_BYTE;
	packet.buf.mem = index;

	/*
	 * Convert the data byte into an uint8_t*
	 */
	packet.buf.data = (uint8_t*)&(data);

	/*
	 * Write the byte
	 */
	if(I2C_BUS_BYTE == write(addr, &packet)){

		/*
		 * All is good return
		 */
		return;
	}else{

		/*
		 * The number of read bytes does not correspond
		 * to the bytes that are in the queue.
		 *
		 * Return and set the status to error.
		 */
		status = ERR_IO_ERROR;
		return;
	}
}

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
bool bus_i2c::probe(){

	status = ERR_UNSUPPORTED_DEV;
	return(false);
}

/*!
 * \brief Returns the next byte to read.
 *
 * This routine uses the buffer to pop the next byte in the receive queue.
 *
 * @return uint8_t		The next byte to address.
 */
uint8_t bus_i2c::peek(){
	return (busif->peek() << 8);
}

/*!
 * \brief Deletes the entries in the queue.
 *
 * This routine uses the buffer to flush all entries in the receive queue.
 */
void bus_i2c::flush(){
	return busif->flush();
}

/*!
 * \brief Checks the numebr of bytes in the receive queue.
 *
 * This routine uses the number of elements in the recieve queue counter
 * to return the amount of elements currently in the receive queue.
 *
 * @return int		The number of bytes in the receive queue.
 */
int bus_i2c::available(){
	return busif->available();
}

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
 * \param	index	The memory index to get
 * \param   mask    The mask of the field to set.
 *
 * \return  The value stored in the register or memory field.
 */
uint8_t bus_i2c::reg_fieldget(uint8_t addr, uint8_t index, uint8_t mask){

	uint8_t const value = mask & get(addr, index);
	return (value / (mask & ~(mask << 1)));
}

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
void bus_i2c::reg_fieldset(uint8_t addr, uint8_t mask, uint8_t value, uint8_t index){

	uint8_t const reg = ~mask & get(addr, index);

	value *= (mask & ~(mask << 1));
	put(addr, index, reg | (value & mask));
}

/*!
 * \brief Gets the internal data pointer
 *
 * @param  rec		The structure pointer to copy the packet in
 * @param packet	The packet read from the bus interface
 * @param  size		The size of the passed structure
 */
void bus_i2c::get_data(void* rec, i2c_bus_packet_t* packet, size_t size){

	/*
	 * We take the the structure pointer and copy "size" data
	 * into it to fill it up.
	 */
	memcpy(packet->buf.data, rec, size);
}

/*
 * Utility function to create / return packets.
 */

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
void packetize(size_t length, uint8_t addr, uint8_t* data,
		i2c_bus_packet_t* packet){

	/*
	 * Set the packet attributes
	 */
	packet->size = length;
	packet->buf.mem = addr;
	packet->buf.data = data;
	return;
}



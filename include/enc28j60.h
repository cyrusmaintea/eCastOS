/* DreamShell ##version##

   drivers/enc28j60.h
   Copyright (C) 2011-2014 SWAT
*/

#ifndef ENC28J60_H
#define ENC28J60_H

#include <arch/types.h>

/**
 * \file
 * enc28j60 driver
 *
 * \author SWAT
 */

/**
 * \internal
 * Initializes the SPI interface to the ENC28J60 chip.
 * 
 * \param[in] cs The SPI chip select pin.
 * \param[in] cs The SPI reset pin.
 */
void enc28j60_io_init(int cs, int rs);

/**
 * \internal
 * Forces a reset to the ENC28J60.
 *
 * After the reset a reinitialization is necessary.
 */
void enc28j60_reset();


/**
 * \internal
 * Reads the value of a hardware register.
 *
 * \param[in] address The address of the register to read.
 * \returns The register value.
 */
uint8 enc28j60_read(uint8 address);

/**
 * \internal
 * Writes the value of a hardware register.
 *
 * \param[in] address The address of the register to write.
 * \param[in] value The value to write into the register.
 */
void enc28j60_write(uint8 address, uint8 value);


/**
 * \internal
 * Clears bits in a hardware register.
 *
 * Performs a NAND operation on the current register value
 * and the given bitmask.
 *
 * \param[in] address The address of the register to alter.
 * \param[in] bits A bitmask specifiying the bits to clear.
 */
void enc28j60_clear_bits(uint8 address, uint8 bits);


/**
 * \internal
 * Sets bits in a hardware register.
 *
 * Performs an OR operation on the current register value
 * and the given bitmask.
 *
 * \param[in] address The address of the register to alter.
 * \param[in] bits A bitmask specifiying the bits to set.
 */
void enc28j60_set_bits(uint8 address, uint8 bits);


/**
 * \internal
 * Reads the value of a hardware PHY register.
 *
 * \param[in] address The address of the PHY register to read.
 * \returns The register value.
 */
uint16 enc28j60_read_phy(uint8 address);


/**
 * \internal
 * Writes the value to a hardware PHY register.
 *
 * \param[in] address The address of the PHY register to write.
 * \param[in] value The value to write into the register.
 */
void enc28j60_write_phy(uint8 address, uint16 value);


/**
 * \internal
 * Reads a byte from the RAM buffer at the current position.
 *
 * \returns The byte read from the current RAM position.
 */
uint8 enc28j60_read_buffer_byte();


/**
 * \internal
 * Writes a byte to the RAM buffer at the current position.
 *
 * \param[in] b The data byte to write.
 */
void enc28j60_write_buffer_byte(uint8 b);


/**
 * \internal
 * Reads multiple bytes from the RAM buffer.
 *
 * \param[out] buffer A pointer to the buffer which receives the data.
 * \param[in] buffer_len The buffer length and number of bytes to read.
 */
void enc28j60_read_buffer(uint8* buffer, uint16 buffer_len);


/**
 * \internal
 * Writes multiple bytes to the RAM buffer.
 *
 * \param[in] buffer A pointer to the buffer containing the data to write.
 * \param[in] buffer_len The number of bytes to write.
 */
void enc28j60_write_buffer(const uint8* buffer, uint16 buffer_len);


/**
 * Switches the hardware register bank.
 *
 * \param[in] num The index of the register bank to switch to.
 */
void enc28j60_bank(uint8 num);


/**
 * Reset and initialize the ENC28J60 and starts packet transmission/reception.
 *
 * \param[in] mac A pointer to a 6-byte buffer containing the MAC address.
 * \returns \c true on success, \c false on failure.
 */
int enc28j60_init(const uint8* mac);


/**
 * Fetches a pending packet from the RAM buffer of the ENC28J60.
 *
 * The packet is written into the given buffer and the size of the packet
 * (ethernet header plus payload, exclusive the CRC) is returned.
 *
 * Zero is returned in the following cases:
 * - There is no packet pending.
 * - The packet is too large to completely fit into the buffer.
 * - Some error occured.
 *
 * \param[out] buffer The pointer to the buffer which receives the packet.
 * \param[in] buffer_len The length of the buffer.
 * \returns The packet size in bytes on success, \c 0 in the cases noted above.
 */
uint16 enc28j60_receive_packet(uint8* buffer, uint16 buffer_len);



/**
 * Writes a packet to the RAM buffer of the ENC28J60 and starts transmission.
 *
 * The packet buffer contains the ethernet header and the payload without CRC.
 * The checksum is automatically generated by the on-chip calculator.
 *
 * \param[in] buffer A pointer to the buffer containing the packet to be sent.
 * \param[in] buffer_len The length of the ethernet packet header plus payload.
 * \returns \c true if the packet was sent, \c false otherwise.
 */
int enc28j60_send_packet(const uint8* buffer, uint16 buffer_len);


/**
 * Initialize network interface
 * 
 * \param[in] cs The SPI chip select pin.
 * \param[in] cs The SPI reset pin.
 * \returns \c true if initialized, \c false otherwise.
 */
int enc28j60_if_init(int cs, int rs);

/**
 * Shutdown network interface
 * 
 * \returns \c true if shutdown success, \c false otherwise.
 */
int enc28j60_if_shutdown();



#endif /* ENC28J60_H */


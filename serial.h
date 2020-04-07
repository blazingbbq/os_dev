#ifndef INCLUDE_SERIAL_H
#define INCLUDE_SERIAL_H

#include "io.h"

/* The I/O ports */

/* All the I/O ports are calculated relative to the data port. This is because
    * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
    * order, but they start at different values.
    */

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */
#define SERIAL_COM2_BASE                0x2F8      /* COM2 base port */
#define SERIAL_COM3_BASE                0x3E8      /* COM3 base port */
#define SERIAL_COM4_BASE                0x2E8      /* COM4 base port */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
    * Tells the serial port to expect first the highest 8 bits on the data port,
    * then the lowest 8 bits will follow
    */
#define SERIAL_LINE_ENABLE_DLAB         0x80

/** serial_easy_init:
 *  Configures the serial port with default values.
 * 
 *  @param com       The COM port to configure
*/
void serial_easy_init(unsigned short com);

/** serial_write:
 *  Writes the given string to the given COM port
 * 
 *  @param com  The COM port to write to
 *  @param buf String to be output to the COM port
*/
int serial_write(unsigned short com, char *buf);

/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor);

/** serial_configure_line:
 *  Configures the line of the given serial port. The port is set to have a
 *  data length of 8 bits, no parity bits, one stop bit and break control
 *  disabled.
 *
 *  @param com      The serial port to configure
 *  @param value    The config value
 */
void serial_configure_line(unsigned short com, unsigned char value);

/** serial_configure_buffers:
 *  Configure buffers of the given serial port. The buffers are enabled,
 *  cleared (both receive and transmit buffers), and set to 14 byte queue sizes
 *
 *  @param com      The serial port to configure
 *  @param value    The config value
*/
void serial_configure_buffers(unsigned short com, unsigned char value);

/** serial_configure_modem:
 *  Configure RTS and DTS to signal we are ready to send data.
 * 
 *  @param com      The serial port to configure
 *  @param value    The config value
*/
void serial_configure_modem(unsigned short com, unsigned char value);

/** serial_is_transmit_fifo_empty:
 *  Checks whether the transmit FIFO queue is empty or not for the given COM
 *  port.
 *
 *  @param  com The COM port
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int com);

#endif /* INCLUDE_SERIAL_H */

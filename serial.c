#include "serial.h"

void serial_easy_init(unsigned short com) {
    serial_configure_baud_rate(com, 2);
    serial_configure_line(com, 0x03);
    serial_configure_buffers(com, 0xC7);
    serial_configure_modem(com, 0x03);
}

int serial_write(unsigned short com, char *buf, unsigned int len) {
    unsigned int written = 0;
    while (written < len) {
        if (serial_is_transmit_fifo_empty(com)) {
            outb(com, buf[written]);
            written++;
        }
    }

    return len;
}

void serial_configure_baud_rate(unsigned short com, unsigned short divisor) {
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

void serial_configure_line(unsigned short com, unsigned char value) {
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
     * Content: | d | b | prty  | s | dl  |
     */
    outb(SERIAL_LINE_COMMAND_PORT(com), value);
}

void serial_configure_buffers(unsigned short com, unsigned char value) {
    /* Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
     * Content: | lvl | bs | r | dma | clt | clr | e |
    */
   outb(SERIAL_FIFO_COMMAND_PORT(com), value);
}

void serial_configure_modem(unsigned short com, unsigned char value) {
    /* Bit:     | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
     * Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
    */
   outb(SERIAL_MODEM_COMMAND_PORT(com), value);
}

int serial_is_transmit_fifo_empty(unsigned int com)
{
    /* 0x20 = 0010 0000 */
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

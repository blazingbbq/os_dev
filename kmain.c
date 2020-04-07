#include "framebuffer.h"
#include "serial.h"

/* C entrypoint */
void kmain() {
    char fb_buf[] = "Hello, Framebuffer!";
    fb_write(fb_buf);

    char serial_buf[] = "Hello, COM1!\nAnother Line!";
    serial_easy_init(SERIAL_COM1_BASE);
    serial_write(SERIAL_COM1_BASE, serial_buf);
}

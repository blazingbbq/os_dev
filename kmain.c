#include "framebuffer.h"
#include "serial.h"
#include "gdt.h"

/* C entrypoint */
void kmain() {
    /* Set GDT */
    struct gdt GDT  = {0x00000000, 0xFFFF};
    set_gdt(GDT);

    char fb_buf[] = "Hello, Framebuffer!";
    fb_write(fb_buf);

    char serial_buf[] = "Hello, COM1!\nAnother Line!";
    serial_easy_init(SERIAL_COM1_BASE);
    serial_write(SERIAL_COM1_BASE, serial_buf);
}

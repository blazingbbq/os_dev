#include "framebuffer.h"
#include "serial.h"
#include "gdt.h"
#include "isr.h"
#include "keyboard.h"

/* C entrypoint */
void kmain() {
    /* Set GDT */
    set_gdt();
    /* Init ISRs */
    isr_init();
    /* Init keyboard driver */
    keyboard_init();

    fb_write("Hello! ");

    serial_easy_init(SERIAL_COM1_BASE);
    serial_write(SERIAL_COM1_BASE, "Hello, COM1!");
}


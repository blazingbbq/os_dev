#include "framebuffer.h"
#include "serial.h"
#include "gdt.h"
#include "isr.h"

/* C entrypoint */
void kmain() {
    /* Set GDT */
    set_gdt();
    /* Init ISRs */
    isr_init();

    char buf[] = "Hello, World!\n";
    fb_write(buf);

    char serial_buf[] = "Hello, COM1!\nAnother Line!";
    serial_easy_init(SERIAL_COM1_BASE);
    serial_write(SERIAL_COM1_BASE, serial_buf);

    /* Trigger interrupt 1 */
    __asm__ __volatile__("int $1");
}

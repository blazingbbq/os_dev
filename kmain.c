#include "framebuffer.h"
#include "serial.h"
#include "gdt.h"
#include "isr.h"
#include "keyboard.h"
#include "multiboot.h"
#include "util.h"
#include "paging.h"
#include "kheap.h"

typedef void (*start_module_t)(void);

/* C entrypoint */
void kmain(u32 ebx) {
  multiboot_info_t *mbi = (multiboot_info_t *) ebx;

  /* Set GDT */
  fb_write("Initializing GDT...\n");
  set_gdt();
  /* Init ISRs */
  fb_write("Initializing interrupts...\n");
  isr_init();
  /* Init keyboard driver */
  fb_write("Initializing keyboard driver...\n");
  keyboard_init();
  /* Init serial COM1 */
  fb_write("Initializing serial port...\n");
  serial_easy_init(SERIAL_COM1_BASE);
  serial_write(SERIAL_COM1_BASE, "Hello, COM1!\n");

  char buf[16];

  u32 a = kmalloc(8);

  fb_write("Initializing paging...\n");
  /* paging_init(); */

  u32 b = kmalloc(8);
  u32 c = kmalloc(8);

  fb_write("Allocating memory...\n");

  itoa(a, buf, 16);
  fb_write("\na: ");
  fb_write(buf);
  fb_write("\n");

  itoa(b, buf, 16);
  fb_write("b: ");
  fb_write(buf);
  fb_write("\n");

  itoa(c, buf, 16);
  fb_write("c: ");
  fb_write(buf);
  fb_write("\n");

  /* kfree((u32 *) c); */
  /* kfree((u32 *) b); */
  /* fb_write("Freed b and c\n"); */
  /* u32 d = kmalloc(12); */
/*  */
  /* itoa(d, buf, 16); */
  /* fb_write("d: "); */
  /* fb_write(buf); */
  /* fb_write("\n"); */

  /* Start the first GRUB module */
  fb_write("Starting first GRUB module...\n");
  multiboot_module_t *mod1 = (multiboot_module_t *) mbi->mods_addr;
  start_module_t start_first_module = (start_module_t) mod1->mod_start;
  start_first_module();

  fb_write("\n");
  fb_write("Hello, World!");
}


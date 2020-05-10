#include "framebuffer.h"
#include "serial.h"
#include "gdt.h"
#include "isr.h"
#include "keyboard.h"
#include "multiboot.h"
#include "util.h"

typedef void (*start_module_t)(void);

/* C entrypoint */
void kmain(u32 ebx) {
  /* Set GDT */
  set_gdt();
  /* Init ISRs */
  isr_init();
  /* Init keyboard driver */
  keyboard_init();
  /* Init serial COM1 */
  serial_easy_init(SERIAL_COM1_BASE);
  serial_write(SERIAL_COM1_BASE, "Hello, COM1!");

  fb_write("Hello, World!\n");

  /* Start the first GRUB module */
  multiboot_info_t *mbi = (multiboot_info_t *) ebx;
  module_t *mod1 = (module_t *) mbi->mods_addr;

  start_module_t start_first_module = (start_module_t) mod1->mod_start;
  start_first_module();
}


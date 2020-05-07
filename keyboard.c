#include "keyboard.h"
#include "io.h"
#include "framebuffer.h"
#include "util.h"

key_handler_t key_handlers[NUM_SCANCODES];

const char * ASCII_SCANCODES[] = {
  "", "ESC", "1", "2",
  "3", "4", "5", "6",
  "7", "8", "9", "0",
  "-", "=", "BKSP", "TAB",
  "q", "w", "e", "r",
  "t", "y", "u", "i",
  "o", "p", "[", "]",
  "ENTR", "L_CTRL", "a", "s",
  "d", "f", "g", "h",
  "j", "k", "l", ";",
  "'", "`", "L_SHFT", "\\",
  "z", "x", "c", "v",
  "b", "n", "m", ",",
  ".", "/", "R_SHFT", "*",
  "L_ALT", " ", "CPS_LCK", "F1",
  "F2", "F3", "F4", "F5",
  "F6", "F7", "F8", "F9",
  "F10", "NUM_LCK", "SCRL_LCK", "7",
  "8", "9", "-", "4",
  "5", "6", "+", "1",
  "2", "3", "0", ".",
  "", "", "", "F11",
  "F12", "", "", "",
  "", "ESC"
};

u8 read_scan_code(void){
  return inb(KBD_DATA_PORT);
}

void keyboard_init() {
  register_interrupt_handler(KBD_INTERRUPT, keyboard_interrupt_handler);
}

void keyboard_interrupt_handler(interrupt_state_t state) {
  (void) state;
  u8 scancode = read_scan_code();

  if (key_handlers[scancode]) {
    key_handler_t handler = key_handlers[scancode];
    handler(scancode);
  } else if (scancode < RELEASE_SCANCODES) {
    char buf[16];
    scancode_to_ascii(scancode, buf);

    fb_write(buf);
  }
}

void register_key_handler(u8 scancode, key_handler_t handler) {
  key_handlers[scancode] = handler;
}

void scancode_to_ascii(u8 scancode, char buf[]) {
  /* buf set but not used */
  (void) buf;
  if (ASCII_SCANCODES[scancode]) {
    strcpy(buf, ASCII_SCANCODES[scancode]);
  }
  /* switch (scancode) { */
    /* case 0x01: */
      /* strcpy(buf, "ESC"); */
      /* break; */
    /* case 0x02: */
      /* strcpy(buf, "1"); */
      /* break; */
    /* case 0x03: */
      /* strcpy(buf, "2"); */
      /* break; */
    /* case 0x04: */
      /* strcpy(buf, "3"); */
      /* break; */
    /* case 0x05: */
      /* strcpy(buf, "4"); */
      /* break; */
    /* case 0x06: */
      /* strcpy(buf, "5"); */
      /* break; */
    /* case 0x07: */
      /* strcpy(buf, "6"); */
      /* break; */
    /* case 0x08: */
      /* strcpy(buf, "7"); */
      /* break; */
    /* case 0x09: */
      /* strcpy(buf, "8"); */
      /* break; */
    /* case 0x0a: */
      /* strcpy(buf, "9"); */
      /* break; */
    /* case 0x0b: */
      /* strcpy(buf, "0"); */
      /* break; */
    /* case 0x0c: */
      /* strcpy(buf, "-"); */
      /* break; */
    /* case 0x0d: */
      /* strcpy(buf, "="); */
      /* break; */
    /* case 0x0e: */
      /* strcpy(buf, "BCKSP"); */
      /* break; */
    /* case 0x0f: */
      /* strcpy(buf, "TAB"); */
      /* break; */
/*  */
    /* default: ; */
      /* strcpy(buf, "default"); */
      /* break; */
  /* } */
}


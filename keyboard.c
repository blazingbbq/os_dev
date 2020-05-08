#include "keyboard.h"
#include "io.h"
#include "framebuffer.h"
#include "util.h"

key_handler_t key_handlers[NUM_SCANCODES];
/* Can register combinations of mod keys by registering ctrl
 * or alt key handler and checking for the other desired mods.
 *
 * Ctrl handlers take precedence over alt handlers when both
 * keys are down.
 */
key_handler_t ctrl_key_handlers[NUM_SCANCODES];
key_handler_t alt_key_handlers[NUM_SCANCODES];

u8 keyboard_shift = 0;
u8 keyboard_ctrl = 0;
u8 keyboard_alt = 0;

void toggle_shift(u8 scancode);
void toggle_ctrl(u8 scancode);
void toggle_alt(u8 scancode);

void ctrl_q_handler(u8 scancode);
void backspace_handler(u8 scancode);

const char * const ASCII_SCANCODES[] = {
  "", "ESC", "1", "2",
  "3", "4", "5", "6",
  "7", "8", "9", "0",
  "-", "=", "BKSP", "TAB",
  "q", "w", "e", "r",
  "t", "y", "u", "i",
  "o", "p", "[", "]",
  "\n", "L_CTRL", "a", "s",
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

const char * const ASCII_SCANCODES_SHIFTED[] = {
  "", "", "!", "@",
  "#", "$", "%", "^",
  "&", "*", "(", ")",
  "_", "+", "", "",
  "Q", "W", "E", "R",
  "T", "Y", "U", "I",
  "O", "P", "{", "}",
  "\n", "", "A", "S",
  "D", "F", "G", "H",
  "J", "K", "L", ":",
  "\"", "~", "", "|",
  "Z", "X", "C", "V",
  "B", "N", "M", "<",
  ">", "?", "", "",
  "", " ", "", "",
  "", "", "", "",
  "", "", "", "",
  "", "", "", "",
  "", "", "", "",
  "", "", "", "",
  "", "", "", "",
  "", "", "", "",
  "", "", "", "",
  "", ""
};


u8 read_scan_code(void){
  return inb(KBD_DATA_PORT);
}

void keyboard_init() {
  register_interrupt_handler(KBD_INTERRUPT, keyboard_interrupt_handler);

  /* Register mod key handlers */
  register_key_handler(L_SHIFT_SCANCODE, toggle_shift);
  register_key_handler(R_SHIFT_SCANCODE, toggle_shift);
  register_key_handler(RELEASE_SCANCODE(L_SHIFT_SCANCODE), toggle_shift);
  register_key_handler(RELEASE_SCANCODE(R_SHIFT_SCANCODE), toggle_shift);

  register_key_handler(L_CTRL_SCANCODE, toggle_ctrl);
  /* register_key_handler(R_CTRL_SCANCODE, toggle_ctrl); */
  register_key_handler(RELEASE_SCANCODE(L_CTRL_SCANCODE), toggle_ctrl);
  /* register_key_handler(RELEASE_SCANCODE(R_CTRL_SCANCODE), toggle_ctrl); */

  register_key_handler(L_ALT_SCANCODE, toggle_alt);
  /* register_key_handler(R_ALT_SCANCODE, toggle_alt); */
  register_key_handler(RELEASE_SCANCODE(L_ALT_SCANCODE), toggle_alt);
  /* register_key_handler(RELEASE_SCANCODE(R_ALT_SCANCODE), toggle_alt); */

  register_ctrl_handler(0x10, ctrl_q_handler);
  register_key_handler(0x0e, backspace_handler);
}

void keyboard_interrupt_handler(interrupt_state_t state) {
  (void) state;
  u8 scancode = read_scan_code();

  if (keyboard_ctrl && ctrl_key_handlers[scancode]) {
    key_handler_t handler = ctrl_key_handlers[scancode];
    handler(scancode);
  } else if (keyboard_alt && alt_key_handlers[scancode]) {
    key_handler_t handler = alt_key_handlers[scancode];
    handler(scancode);
  } else if (key_handlers[scancode]) {
    key_handler_t handler = key_handlers[scancode];
    handler(scancode);
  } else if (scancode < RELEASE_OFFSET) {
    char buf[16];
    scancode_to_ascii(scancode, keyboard_shift, buf);

    fb_write(buf);
  }
}

void register_key_handler(u8 scancode, key_handler_t handler) {
  key_handlers[scancode] = handler;
}

void register_ctrl_handler(u8 scancode, key_handler_t handler) {
  ctrl_key_handlers[scancode] = handler;
}

void register_alt_handler(u8 scancode, key_handler_t handler) {
  alt_key_handlers[scancode] = handler;
}

void scancode_to_ascii(u8 scancode, u8 shifted, char buf[]) {
  /* buf set but not used */
  (void) buf;

  if ((shifted)
      ? ASCII_SCANCODES_SHIFTED[scancode]
      : ASCII_SCANCODES[scancode]) {
    strcpy(buf, (shifted)
        ? ASCII_SCANCODES_SHIFTED[scancode]
        : ASCII_SCANCODES[scancode]);
  }
}

void toggle_shift(u8 scancode) {
  switch (scancode) {
    case L_SHIFT_SCANCODE:
    case R_SHIFT_SCANCODE:
      keyboard_shift = 1;
      break;
    case RELEASE_SCANCODE(L_SHIFT_SCANCODE):
    case RELEASE_SCANCODE(R_SHIFT_SCANCODE):
      keyboard_shift = 0;
      break;
  }
}

void toggle_ctrl(u8 scancode) {
  switch (scancode) {
    case L_CTRL_SCANCODE:
    /* case R_CTRL_SCANCODE: */
      keyboard_ctrl = 1;
      break;
    case RELEASE_SCANCODE(L_CTRL_SCANCODE):
    /* case RELEASE_SCANCODE(R_CTRL_SCANCODE): */
      keyboard_ctrl = 0;
      break;
  }
}

void toggle_alt(u8 scancode) {
  switch (scancode) {
    case L_ALT_SCANCODE:
    /* case R_ALT_SCANCODE: */
      keyboard_alt = 1;
      break;
    case RELEASE_SCANCODE(L_ALT_SCANCODE):
    /* case RELEASE_SCANCODE(R_ALT_SCANCODE): */
      keyboard_alt = 0;
      break;
  }
}

void ctrl_q_handler(u8 scancode) {
  (void) scancode;

  /* TODO: exit(0) */
  fb_write("CTRL+Q");
}

void backspace_handler(u8 scancode) {
  (void) scancode;

  fb_backspace();
}


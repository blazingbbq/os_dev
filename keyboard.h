#ifndef INCLUDE_KEYBOARD_H
#define INCLUDE_KEYBOARD_H

#include "isr.h"
#include "types.h"

#define KBD_DATA_PORT       0x60
#define KBD_INTERRUPT       33
#define NUM_SCANCODES       0xFF
#define RELEASE_SCANCODES   0x80

typedef void (*key_handler_t)(u8 scancode);

/** read_scan_code:
 *  Reads scan code from keyboard data port
 *
 *  @return The scan code (NOT an ASCII character).
 */
u8 read_scan_code(void);

/** keyboard_init
 *  Initialize keyboard driver. Registers interrupt handler
 */
void keyboard_init();

/** keyboard_interrupt_handler
 *  Handler for keyboard interrupts
 *
 *  @param state  Registers state at time of interrupt
 */
void keyboard_interrupt_handler(interrupt_state_t state);

/** register_key_handler
 *  Register key handler for specified scancode
 *
 *  @param scancode   The scancode for which to register the handler
 *  @param handler    The callback function to handle the scancode keypress
 */
void register_key_handler(u8 scancode, key_handler_t handler);

/** scancode_to_ascii
 *  Convert keyboard scancode to ascii
 *
 *  @param scancode   Scancode to convert
 *  @param buf        Buffer to store converted value
 */
void scancode_to_ascii(u8 scancode, char buf[]);

#endif /* INCLUDE_KEYBOARD_H */

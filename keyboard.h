#ifndef INCLUDE_KEYBOARD_H
#define INCLUDE_KEYBOARD_H

#include "isr.h"
#include "types.h"

#define KBD_DATA_PORT         0x60
#define KBD_INTERRUPT         33
#define NUM_SCANCODES         0xFF
#define RELEASE_OFFSET        0x80
#define RELEASE_SCANCODE(sc)  (sc + RELEASE_OFFSET)

#define L_SHIFT_SCANCODE      0x2a
#define R_SHIFT_SCANCODE      0x36
#define L_CTRL_SCANCODE       0x1d
#define R_CTRL_SCANCODE
#define L_ALT_SCANCODE        0x38
#define R_ALT_SCANCODE

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

/** register_ctrl_handler
 *  Register key handler for specified scancode when ctrl is held
 *
 *  @param scancode   The scancode for which to register the handler
 *  @param handler    The callback function to handle the scancode keypress
 */
void register_ctrl_handler(u8 scancode, key_handler_t handler);

/** register_alt_handler
 *  Register key handler for specified scancode when alt is held
 *
 *  @param scancode   The scancode for which to register the handler
 *  @param handler    The callback function to handle the scancode keypress
 */
void register_alt_handler(u8 scancode, key_handler_t handler);

/** scancode_to_ascii
 *  Convert keyboard scancode to ascii
 *
 *  @param scancode   Scancode to convert
 *  @param shifted    Whether the shifted key value should be returned
 *  @param buf        Buffer to store converted value
 */
void scancode_to_ascii(u8 scancode, u8 shifted, char buf[]);

#endif /* INCLUDE_KEYBOARD_H */

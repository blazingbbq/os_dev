#ifndef INCLUDE_FRAMEBUFFER_H
#define INCLUDE_FRAMEBUFFER_H

#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

#define FB_ROWS                 25
#define FB_COLS                 80

/** fb_write:
 *  Writes given string to the framebuffer. Automatically advances cursor and
 *  scrolls if necessary.
 * 
 *  @param buf String to be output to framebuffer
*/
int fb_write(char *buf);

/** fb_scroll_by:
 *  Scroll framebuffer test by given number of lines
 * 
 *  @param lines The number of lines to scroll by
*/
void fb_scroll_by(unsigned int lines);

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos);

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

#endif /* INCLUDE_FRAMEBUFFER_H */

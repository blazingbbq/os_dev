#include "framebuffer.h"

/* Starting address for the framebuffer memory-mapped IO */
char *fb = (char *) 0xC00B8000;
unsigned short fb_curr_pos = 0x00000000;

void advance_cursor();

int fb_write(char *buf) {
  unsigned int i = 0;
  while (buf[i] != 0) {
    switch(buf[i]) {
      case '\n':
        fb_move_cursor(fb_curr_pos + FB_COLS - fb_curr_pos % FB_COLS);
        break;
      default:
        fb_write_cell(fb_curr_pos * 2, buf[i], (unsigned char) 15, (unsigned char) 0);
        advance_cursor();
        break;
    }

    if (fb_curr_pos >= FB_ROWS * FB_COLS) {
      fb_scroll_by(1);
    }
    i++;
  }

  return 0;
}

void fb_backspace() {
  if (fb_curr_pos <= 0) return;

  /* TODO: Handle backspace across newline */
  fb_move_cursor(fb_curr_pos - 1);
  fb_write_cell(fb_curr_pos * 2, ' ', (unsigned char) 15, (unsigned char) 0);
}

void fb_scroll_by(unsigned int lines) {
  unsigned int i;
  for (i = 0; i < (FB_ROWS - lines) * FB_COLS * 2; i++) {
    fb[i] = fb[i + FB_COLS * 2];
  }
  for (i = i; i < FB_ROWS * FB_COLS * 2; i++) {
    fb[i] = 0;
  }
  fb_move_cursor(fb_curr_pos - FB_COLS * lines);
}

void fb_move_cursor(unsigned short pos) {
  outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
  outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
  outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
  outb(FB_DATA_PORT,    pos & 0x00FF);
  fb_curr_pos = pos;
}

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg) {
  fb[i] = c;
  fb[i + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

void advance_cursor() {
  fb_move_cursor(fb_curr_pos + 1);
}


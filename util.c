#include "util.h"

void strcpy(char dest[], const char src[]) {
  int i = 0;
  while (src[i] != 0) {
    dest[i] = src[i];
    i++;
  }
  /* null terminate string */
  dest[i] = '\0';
}

void itoa(u32 num, char dest[], u32 base) {
  /* Guard for base 0, since we divide by base */
  if (base == 0) return;

  u32 i = 0;
  
  /* Use do..while to handle 0 case */
  do {
    u32 remainder = num % base;
    dest[i++] = (remainder > 9) ? 'a' + remainder - 10 : '0' + remainder;
    num /= base;
  } while (num != 0);

  /* FIXME: Might want to remove these base specific prefixes */
  switch (base) {
    case 2:
      dest[i++] = 'b';
      dest[i++] = '0';
      break;
    case 16:
      dest[i++] = 'x';
      dest[i++] = '0';
      break;
  }

  /* null terminate string  */
  dest[i] = '\0';

  str_reverse(dest, i);
}

void str_reverse(char str[], u32 len) {
  for (u32 i = 0; i < len / 2; i++) {
    char tmp = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = tmp;
  }
}

u32* memset(u32 *dest, u32 val, u32 len) {
  u32 *ptr = (u32 *) dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}


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


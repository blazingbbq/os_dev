#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

#include "types.h"

/** strcpy
 *  Copy string from one char array to another
 *
 *  @param dest     Array to copy string to
 *  @param source   String to be copied
 */
void strcpy(char dest[], const char src[]);

/** itoa
 *  Convert number to string representation in given base
 *
 *  @param num      Number to convert to string
 *  @param dest     Destination buffer to store converted string
 *  @param base     Base to represent number in (eg. 10 for decimal)
 */
void itoa(u32 num, char dest[], u32 base);

/** str_reverse
 *  Reverse given string
 *
 *  @param str      Buffer containing the string to be reversed
 *  @param len      Length of the string to be reversed
 */
void str_reverse(char str[], u32 len);

/** memset
 *  Set value of memory to given value
 *
 *  @param dest   Memory to set
 *  @param val    Value to set memory to
 *  @param len    Length of memory region to set
 *
 *  @return       The address of the memory region set
 */
u32* memset(u32 *dest, u32 val, u32 len);

#endif /* INCLUDE_UTIL_H */

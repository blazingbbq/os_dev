#ifndef INCLUDE_IDT_H
#define INCLUDE_IDT_H

#include "types.h"

/** idt_info
 *  Struct representing the info passed to load_idt
 */
typedef struct {
  u16 size;
  u32 offset;
}__attribute__((packed)) idt_info_t;

/** idt_gate
 *  Struct representing IDT gate entries
 */
typedef struct {
  u16 offset_1;  // offset bits 0..15, lower 16 bits of handler address
  u16 selector;  // code segment selector
  u8  zero;      // unused, always 0
  u8  type_attr; // type and attributes
  u16 offset_2;  // offset bits 16..31, upper 16  bits of handler address
}__attribute__((packed)) idt_gate_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_info_t idt_inf;


/** load_idt
 *  Load the Interrupt Descriptor Table
 *
 *  @param info Address of struct containing size and base address of IDT entries
 */
void load_idt(idt_info_t info);

#endif /* INCLUDE_IDT_H */

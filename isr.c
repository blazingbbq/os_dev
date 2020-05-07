#include "isr.h"
#include "idt.h"
#include "framebuffer.h"
#include "types.h"
#include "io.h"

void isr_init() {
  /* Set IDT gate entries, handlers are defined in idt.s */
  set_idt_gate(0, (u32) interrupt_handler_0);
  set_idt_gate(1, (u32) interrupt_handler_1);
  set_idt_gate(2, (u32) interrupt_handler_2);
  set_idt_gate(3, (u32) interrupt_handler_3);
  set_idt_gate(4, (u32) interrupt_handler_4);
  set_idt_gate(5, (u32) interrupt_handler_5);
  set_idt_gate(6, (u32) interrupt_handler_6);
  set_idt_gate(7, (u32) interrupt_handler_7);
  set_idt_gate(8, (u32) interrupt_handler_8);
  set_idt_gate(9, (u32) interrupt_handler_9);
  set_idt_gate(10, (u32) interrupt_handler_10);
  set_idt_gate(11, (u32) interrupt_handler_11);
  set_idt_gate(12, (u32) interrupt_handler_12);
  set_idt_gate(13, (u32) interrupt_handler_13);
  set_idt_gate(14, (u32) interrupt_handler_14);
  set_idt_gate(15, (u32) interrupt_handler_15);
  set_idt_gate(16, (u32) interrupt_handler_16);
  set_idt_gate(17, (u32) interrupt_handler_17);
  set_idt_gate(18, (u32) interrupt_handler_18);
  set_idt_gate(19, (u32) interrupt_handler_19);
  set_idt_gate(20, (u32) interrupt_handler_20);
  set_idt_gate(21, (u32) interrupt_handler_21);
  set_idt_gate(22, (u32) interrupt_handler_22);
  set_idt_gate(23, (u32) interrupt_handler_23);
  set_idt_gate(24, (u32) interrupt_handler_24);
  set_idt_gate(25, (u32) interrupt_handler_25);
  set_idt_gate(26, (u32) interrupt_handler_26);
  set_idt_gate(27, (u32) interrupt_handler_27);
  set_idt_gate(28, (u32) interrupt_handler_28);
  set_idt_gate(29, (u32) interrupt_handler_29);
  set_idt_gate(30, (u32) interrupt_handler_30);
  set_idt_gate(31, (u32) interrupt_handler_31);

  /* Remap PIC interrupts to interrupts 32..40 */
  PIC_remap(0x20, 0x28);

  /* Set gate entries for ISR handlers */
  set_idt_gate(32, (u32) interrupt_handler_32);
  set_idt_gate(33, (u32) interrupt_handler_33);
  set_idt_gate(34, (u32) interrupt_handler_34);
  set_idt_gate(35, (u32) interrupt_handler_35);
  set_idt_gate(36, (u32) interrupt_handler_36);
  set_idt_gate(37, (u32) interrupt_handler_37);
  set_idt_gate(38, (u32) interrupt_handler_38);
  set_idt_gate(39, (u32) interrupt_handler_39);
  set_idt_gate(40, (u32) interrupt_handler_40);
  set_idt_gate(41, (u32) interrupt_handler_41);
  set_idt_gate(42, (u32) interrupt_handler_42);
  set_idt_gate(43, (u32) interrupt_handler_43);
  set_idt_gate(44, (u32) interrupt_handler_44);
  set_idt_gate(45, (u32) interrupt_handler_45);
  set_idt_gate(46, (u32) interrupt_handler_46);
  set_idt_gate(47, (u32) interrupt_handler_47);

  idt_inf.offset = (u32) &idt;
  idt_inf.size = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
  load_idt(idt_inf);
}

void set_idt_gate(int n, u32 handler) {
  idt[n].offset_1 = (u16)((handler) & 0xFFFF);
  idt[n].selector = 0x08;
  idt[n].zero = 0;
  idt[n].type_attr = 0x8E;
  idt[n].offset_2 = (u16)(((handler) >> 16) & 0xFFFF);
}

void interrupt_handler(interrupt_state_t state) {
  // TODO: Handle common isr and dispatch to correct handlers
  if (state.interrupt > 31)
    PIC_sendEOI(state.interrupt - 32);

  char buf[] = "Interrupt handler!\n";
  fb_write(buf);
}

void PIC_remap(int offset1, int offset2) {
  u8 a1, a2;

  /* Save masks */
  a1 = inb(PIC1_DATA);
  a2 = inb(PIC2_DATA);

  /* Start init sequence, in cascade mode */
  outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
  outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
  /* ICW2: vector offset */
  outb(PIC1_DATA, offset1);
  outb(PIC2_DATA, offset2);
  /* ICW3 */
  outb(PIC1_DATA, 4);
  outb(PIC2_DATA, 2);

  outb(PIC1_DATA, ICW4_8086);
  outb(PIC2_DATA, ICW4_8086);

  /* Restore saved masks */
  outb(PIC1_DATA, a1);
  outb(PIC2_DATA, a2);
}

void PIC_sendEOI(u8 irq) {
  if (irq >= 8)
    outb(PIC2_COMMAND, PIC_EOI);

  outb(PIC1_COMMAND, PIC_EOI);
}


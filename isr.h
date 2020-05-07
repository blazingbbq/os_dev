#ifndef INCLUDE_ISR_H
#define INCLUDE_ISR_H

#include "idt.h"
#include "types.h"

#define PIC1          0x20
#define PIC2          0xA0
#define PIC1_COMMAND  PIC1
#define PIC1_DATA     (PIC1 + 1)
#define PIC2_COMMAND  PIC2
#define PIC2_DATA     (PIC2 + 1)

#define ICW1_ICW4	      0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	    0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	    0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	      0x10		/* Initialization - required! */

#define ICW4_8086	      0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	      0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	      0x10		/* Special fully nested (not) */

#define PIC_EOI         0x20    /* End-of-interrupt command code */

typedef struct {
  u32 eax;
  u32 ebx;
  u32 ecx;
  u32 edx;
  u32 esp;
  u32 ebp;
  u32 esi;
  u32 edi;
}__attribute__((packed)) cpu_state_t;

typedef struct {
  u32 error_code;
  u32 eip;
  u32 cs;
  u32 eflags;
}__attribute__((packed)) stack_state_t;

typedef struct {
  cpu_state_t cpu;
  u32 interrupt;
  stack_state_t stack;
}__attribute((packed)) interrupt_state_t;

typedef void (*isr_t)(interrupt_state_t);

/** isr_init
 *  Initialize interupt service routines and load IDT
 */
void isr_init();

/** set_idt_gate
 *  Set gate entry in idt
 *
 *  @param n        Interrupt number
 *  @param handler  Handler function for interrupt
 */
void set_idt_gate(int n, u32 handler);

/** register_interrupt_handler
 *  Register handler function for given interrupt
 *
 *  @param n        Interrupt number
 *  @param handler  Handler function for interrupt
 */
void register_interrupt_handler(int n, isr_t handler);

/** interrupt_handler
 *  Common interrupt handler routine
 *
 *  @param state    Registers state at time of interrupt
 */
void interrupt_handler(interrupt_state_t state);

/** PIC_remap
 *  Remap PIC interrupts
 *
 *  @param offset1  Vector offset for master PIC
 *  @param offset2  Vector offset for slave PIC
 */
void PIC_remap(int offset1, int offset2);

/** PIC_sendEOI
 *  Send End Of Interrupt command to PIC chip(s)
 *
 *  @param irq  Interrupt triggered
 */
void PIC_sendEOI(u8 irq);

/* Interrupt handlers defined in idt.s */
extern void interrupt_handler_0();
extern void interrupt_handler_1();
extern void interrupt_handler_2();
extern void interrupt_handler_3();
extern void interrupt_handler_4();
extern void interrupt_handler_5();
extern void interrupt_handler_6();
extern void interrupt_handler_7();
extern void interrupt_handler_8();
extern void interrupt_handler_9();
extern void interrupt_handler_10();
extern void interrupt_handler_11();
extern void interrupt_handler_12();
extern void interrupt_handler_13();
extern void interrupt_handler_14();
extern void interrupt_handler_15();
extern void interrupt_handler_16();
extern void interrupt_handler_17();
extern void interrupt_handler_18();
extern void interrupt_handler_19();
extern void interrupt_handler_20();
extern void interrupt_handler_21();
extern void interrupt_handler_22();
extern void interrupt_handler_23();
extern void interrupt_handler_24();
extern void interrupt_handler_25();
extern void interrupt_handler_26();
extern void interrupt_handler_27();
extern void interrupt_handler_28();
extern void interrupt_handler_29();
extern void interrupt_handler_30();
extern void interrupt_handler_31();
/* ISRs */
extern void interrupt_handler_32();
extern void interrupt_handler_33();
extern void interrupt_handler_34();
extern void interrupt_handler_35();
extern void interrupt_handler_36();
extern void interrupt_handler_37();
extern void interrupt_handler_38();
extern void interrupt_handler_39();
extern void interrupt_handler_40();
extern void interrupt_handler_41();
extern void interrupt_handler_42();
extern void interrupt_handler_43();
extern void interrupt_handler_44();
extern void interrupt_handler_45();
extern void interrupt_handler_46();
extern void interrupt_handler_47();

#endif /* INCLUDE_ISR_H */

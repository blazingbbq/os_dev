#ifndef INCLUDE_ISR_H
#define INCLUDE_ISR_H

#include "idt.h"
#include "types.h"

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
  stack_state_t stack;
  u32 interrupt;
}__attribute((packed)) interrupt_state_t;

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

/** interrupt_handler
 *  Common interrupt handler routine
 *
 *  @param state    Registers state at time of interrupt
 */
void interrupt_handler(interrupt_state_t state);

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

#endif /* INCLUDE_ISR_H */

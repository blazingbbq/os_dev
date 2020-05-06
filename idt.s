extern interrupt_handler

global load_idt

; load_idt - Loads the interrupt descriptor table (IDT)
; stack: [esp + 4] the address of the first entry in the IDT
;        [esp    ] the return address
load_idt:
  lidt  [ esp + 4 ]                 ; load the IDT
  ret

%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
  cli                               ; disable interrupts
  push  dword 0                     ; push 0 as error code
  push  dword %1                    ; push the interrupt number
  jmp   common_interrupt_handler    ; jump to the common handler
%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
  cli                               ; disable interrupts
  push  dword %1                    ; push the interrupt number
  jmp   common_interrupt_handler    ; jump to the common handler
%endmacro

common_interrupt_handler:
  ; save registers
  push  eax
  push  ebx
  push  ecx
  push  edx
  push  esp
  push  ebp
  push  esi
  push  edi

  ; call interrupt handler
  call  interrupt_handler

  ; restore registers
  pop   edi
  pop   esi
  pop   ebp
  pop   esp
  pop   edx
  pop   ecx
  pop   ebx
  pop   eax

  ; restore esp, since we pushed error_code and interrupt number on the stack
  add   esp, 8
  
  ; re-enable interrupts
  sti

  ; return from interrupt
  iret

; register interrupt handlers
; exceptions
no_error_code_interrupt_handler 0   ; disivion by zero
no_error_code_interrupt_handler 1   ; single-step interrupt
no_error_code_interrupt_handler 2   ; nmi
no_error_code_interrupt_handler 3   ; breakpoint
no_error_code_interrupt_handler 4   ; overflow
no_error_code_interrupt_handler 5   ; bounds
no_error_code_interrupt_handler 6   ; invalid opcode
no_error_code_interrupt_handler 7   ; coprocessor not available
error_code_interrupt_handler 8      ; double fault
no_error_code_interrupt_handler 9   ; coprocessor segment overrun
error_code_interrupt_handler 10     ; invalid task state segment
error_code_interrupt_handler 11     ; segment not present
error_code_interrupt_handler 12     ; stack fault
error_code_interrupt_handler 13     ; general protection fault
error_code_interrupt_handler 14     ; page fault
no_error_code_interrupt_handler 15  ; reserved
no_error_code_interrupt_handler 16  ; math fault
error_code_interrupt_handler 17     ; alignment check
no_error_code_interrupt_handler 18  ; machine check
no_error_code_interrupt_handler 19  ; simd floating-point exception
no_error_code_interrupt_handler 20  ; virtualization exception
no_error_code_interrupt_handler 21  ; control protection exception
no_error_code_interrupt_handler 22  ; reserved
no_error_code_interrupt_handler 23  ; reserved
no_error_code_interrupt_handler 24  ; reserved
no_error_code_interrupt_handler 25  ; reserved
no_error_code_interrupt_handler 26  ; reserved
no_error_code_interrupt_handler 27  ; reserved
no_error_code_interrupt_handler 28  ; reserved
no_error_code_interrupt_handler 29  ; reserved
no_error_code_interrupt_handler 30  ; reserved
no_error_code_interrupt_handler 31  ; reserved

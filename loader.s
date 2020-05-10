bits 32
extern kmain                    ; the C entrypoint
global loader                   ; the entry symbol for ELF

MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
FLAGS        equ 0x00000001     ; multiboot flags: align modules
CHECKSUM     equ -(MAGIC_NUMBER + FLAGS)  ; calculate the checksum
                                          ; (magic number + flags + checksum should equal 0)
KERNEL_STACK_SIZE equ 4096      ; size of stack in bytes

section .grub_sig
signature:
    dd MAGIC_NUMBER             ; write the magic number to the machine code,
    dd FLAGS                    ; the flags,
    dd CHECKSUM                 ; and the checksum

section .text                   ; start of the text (code) section
loader:                         ; the loader label (defined as entry point in linker script)
    mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to the start of the
                                                ; stack (end of memory area)
    ; push function arguments from right-to-left to the stack
    push ebx                    ; GRUB stores address of multiboot information struct in ebx
                                ; pass the address to kmain so it can locate modules
    call kmain                  ; C entrypoint, result will be in eax registe

.loop:
    jmp .loop                   ; loop forever

section .bss
kernel_stack:                   ; label points to beginning of memory
    resb KERNEL_STACK_SIZE      ; reserve stack for the kernel
_sys_stack:

bits 32
extern kmain                    ; the C entrypoint
global loader                   ; the entry symbol for ELF

MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
FLAGS        equ 0x00000001     ; multiboot flags: align modules
CHECKSUM     equ -(MAGIC_NUMBER + FLAGS)  ; calculate the checksum
                                          ; (magic number + flags + checksum should equal 0)
KERNEL_STACK_SIZE   equ 4096              ; size of stack in bytes
KERNEL_VIRTUAL_ADDR equ 0xC0000000        ; virtual address of kernel (3GB)
KERNEL_PDE          equ (KERNEL_VIRTUAL_ADDR >> 22) ; shift page table entry offset and page frame offset
                                                    ; to get the kernel's page directory entry (PDE)

section .grub_sig
signature:
    dd MAGIC_NUMBER             ; write the magic number to the machine code,
    dd FLAGS                    ; the flags,
    dd CHECKSUM                 ; and the checksum

section .text                   ; start of the text (code) section
_loader:                        ; the loader label (defined as entry point in linker script)
loader equ (_loader - KERNEL_VIRTUAL_ADDR) ; Must use physical address until paging is enabled

    mov ecx, (boot_page_directory - KERNEL_VIRTUAL_ADDR)
    mov cr3, ecx                ; set cr3 to the address of the page directory

    mov ecx, cr4                ; read current value of cr4
    or  ecx, 0x00000010         ; set PSE bit to enable 4MB pages
    mov cr4, ecx                ; write updated value of cr4

    mov ecx, cr0                ; read current value of cr0
    or  ecx, 0x80000000         ; set PG bit to enable enable paging
    mov cr0, ecx                ; write updated value of cr0
                                ; paging is enabled now
    lea ecx, [higher_half]      ; load address of the high_half label into ecx
    jmp ecx                     ; jump to the higher half kernel

higher_half:
    ; mov dword [boot_page_directory], 0 ; unmap the identity mapping as it is now unnecessary
    invlpg [0]                         ; invalidate TLB references to virtual address 0, since it has been unmapped

    mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to the start of the
                                                ; stack (end of memory area)
    ; push function arguments from right-to-left to the stack
    add ebx, KERNEL_VIRTUAL_ADDR
    push ebx                      ; GRUB stores address of multiboot information struct in ebx
                                  ; pass the address to kmain so it can locate modules
    call kmain                    ; C entrypoint, result will be in eax registe

.loop:
    jmp .loop                     ; loop forever

section .bss
kernel_stack:                     ; label points to beginning of memory
    resb KERNEL_STACK_SIZE        ; reserve stack for the kernel
_sys_stack:

section .data
align 4096
boot_page_directory:
  dd 0x00000083                     ; identity map the kernel
                                    ; set PS (page size 4MB), RW and P (present)
  times (KERNEL_PDE - 1) dd 0       ; fill the directory (before the kernel directory entry) with empty entries
  dd 0x00000083
  times (1024 - KERNEL_PDE - 1) dd 0 ; fill the rest of the directory with empty entries

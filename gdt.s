global set_gdt

; Define gdt
gdt_start:

gdt_null:   ; mandatory 8 byte null descriptor
  dd  0x0
  dd  0x0

gdt_code:       ; code segment descriptor
  ; 1st flags:  (present)1 (privilege)00 (descriptor type)1           -> 1001b
  ; Type flags: (code)1 (conforming)0 (readable)1 (accessed)0         -> 1010b
  ; 2nd flags:  (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 -> 1100b
  dw  0xffff    ; Limit
  dw  0x0       ; Base 0..15
  db  0x0       ; Base 16..23
  db  10011010b ; 1st flags, type flags
  db  11001111b ; 2nd flags, Limit 16..19
  db  0x0       ; Base 24..31

gdt_data:       ; data segment descriptor
  ; Same flags except for type flags
  ; Type flags: (code)0 (expand down)0 (writable)1 (accessed)0        -> 0010b
  dw  0xffff    ; Limit
  dw  0x0       ; Base 0..15
  db  0x0       ; Base 16..23
  db  10010010b ; 1st flags, type flags
  db  11001111b ; 2nd flags, Limit 16..19
  db  0x0       ; Base 24..31

gdt_end:        ; Used for calculating size of gdt

gdt_descriptor:
  dw  gdt_end - gdt_start - 1 ; Size of gdt, always 1 smaller than actual
  dd  gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

set_gdt:
  lgdt  [gdt_descriptor]
  jmp   CODE_SEG:flush_cs

flush_cs:
  mov   ax, DATA_SEG
  mov   ds, ax
  mov   es, ax
  mov   fs, ax
  mov   gs, ax
  mov   ss, ax
  ret

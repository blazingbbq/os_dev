global set_gdt

set_gdt:
  lgdt [esp + 4]
  ret
  jmp  0x08:flush_cs
flush_cs:
  mov   ax, 0x10
  mov   ds, ax
  mov   es, ax
  mov   fs, ax
  mov   gs, ax
  mov   ss, ax
  ret

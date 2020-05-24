#include "kheap.h"

extern void _kernel_virtual_end();
extern void _kernel_physical_end();

u32 placement_address = (u32) &_kernel_virtual_end;

u32 kmalloc_int(u32 size, u8 align, u32 *addr) {
  /* TODO: Allocate on kernel heap if it exists */

  if (align && (placement_address & 0xFFFF000)) {
    /* Align the placement address */
    placement_address &= 0xFFFFF000;
    placement_address += 0x1000;
  }
  if (addr) {
    *addr = placement_address;
  }
  u32 tmp = placement_address;
  placement_address += size;
  return tmp;
}

u32 kmalloc_a(u32 size) {
  return kmalloc_int(size, 1, 0);
}

u32 kmalloc_p(u32 size, u32 *addr) {
  return kmalloc_int(size, 0, addr);
}

u32 kmalloc_ap(u32 size, u32 *addr) {
  return kmalloc_int(size, 1, addr);
}

u32 kmalloc(u32 size) {
  return kmalloc_int(size, 0, 0);
}


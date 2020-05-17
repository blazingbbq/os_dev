#ifndef INCLUDE_KHEAP_H
#define INCLUDE_KHEAP_H

#include "types.h"

extern void _kernel_virtual_end();

/** kmalloc_a
 *  Allocate chunk of memory. Chunk must be page alligned
 *
 *  @param size   The size of the memory chunk to allocate
 */
u32 kmalloc_a(u32 size);

/** kmalloc_o
 *  Allocate a chunk of memory and return the physical address
 *
 *  @param size   The size of the memory chunk to allocate
 *  @param addr   Physical address of the memory allocated
 */
u32 kmalloc_p(u32 size, u32 *addr);

/** kmalloc_ap
 *  Allocate a chunk of memory and return the physical address. Chunk must be page alligned
 *
 *  @param size   The size of the memory chunk to allocate
 *  @param addr   Physical address of the memory allocated
 */
u32 kmalloc_ap(u32 size, u32 *addr);

/** kmalloc
 *  Allocate a chunk of memory of given size.
 *
 *  @param size   The size of the memory chunk to allocate
 */
u32 kmalloc(u32 size);

#endif /* INCLUDE_KHEAP_H */

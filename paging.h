#ifndef INCLUDE_PAGING_H
#define INCLUDE_PAGING_H

#include "types.h"

#define PAGE_SIZE   0x1000

extern void boot_page_directory();

typedef struct kernel_memory {
  u32 kernel_physical_start;
  u32 kernel_physical_end;
  u32 kernel_virtual_start;
  u32 kernel_virtual_end;
} kernel_memory_t;

typedef struct page {
  u32 present     : 1;
  u32 rw          : 1;
  u32 user        : 1;
  u32 accessed    : 1;
  u32 dirty       : 1;
  u32 unused      : 7;
  u32 frame       : 20;
} page_t;

typedef struct page_table {
  page_t pages[1024];
} page_table_t;

typedef struct page_directory {
  page_table_t *tables[1024];
  u32 tables_physical[1024];
  u32 physical_addr;
} page_directory_t;

void paging_init();

void switch_page_directory(page_directory_t *addr);

page_t *get_page(u32 addr, u32 make, page_directory_t *dir);

#endif /* INCLUDE_PAGING_H */

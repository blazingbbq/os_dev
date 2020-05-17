#include "paging.h"
#include "kheap.h"
#include "util.h"

page_directory_t *kernel_dir;
page_directory_t *current_dir;

/* Bitmap of pages */
u32 *frames;
u32 nframes;

#define INDEX_FROM_BIT(a) (a/(32))
#define OFFSET_FROM_BIT(a) (a%(32))

/* Defined in kheap.c */
extern u32 placement_address;

void set_frame(u32 addr) {
  u32 frame = addr / PAGE_SIZE;
  u32 index = INDEX_FROM_BIT(frame);
  u32 offset = OFFSET_FROM_BIT(frame);
  frames[index] |= ~(0x1 << offset);
}

void clear_frame(u32 addr) {
  u32 frame = addr / PAGE_SIZE;
  u32 index = INDEX_FROM_BIT(frame);
  u32 offset = OFFSET_FROM_BIT(frame);
  frames[index] &= ~(0x1 << offset);
}

u32 frame_present(u32 addr) {
  u32 frame = addr / PAGE_SIZE;
  u32 index = INDEX_FROM_BIT(frame);
  u32 offset = OFFSET_FROM_BIT(frame);
  return (frames[index] & (0x1 << offset));
}

u32 next_available_frame() {
  u32 i, j;
  for (i = 0; i < INDEX_FROM_BIT(nframes); i++) {
    /* Check for available frame */
    if (frames[i] != 0xFFFFFFFF) {
      for (j = 0; j < 32; j++) {
        /* Find available frame in bitmap entry */
        if (!(frames[i] & (0x1 << j))) {
          return i * 32 + j;
        }
      }
    }
  }

  return 0;
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable) {
  /* Check if frame is already allocated */
  if (page->frame != 0)
    return;
  
  u32 index = next_available_frame();
  if (index >= (u32) - 1) {
    /*TODO: Handle no free frames... */
  }

  set_frame(index * PAGE_SIZE);
  page->present = 1;
  page->rw = (is_writeable) ? 1 : 0;
  page->user = (is_kernel) ? 0 : 1;
  page->frame = index;
}

void free_frame(page_t *page) {
  /* Check that page is allocated to frame */
  if (!page->frame)
    return;
  
  u32 frame = page->frame;
  clear_frame(frame);
  page->frame = 0x0;
}

void paging_init() {
  /* Assume 16MB of physical memory for now */
  u32 mem_end_page = 0x1000000;
  
  nframes = mem_end_page / PAGE_SIZE;
  frames = (u32 *) kmalloc(INDEX_FROM_BIT(nframes));
  memset(frames, 0, INDEX_FROM_BIT(nframes));

  kernel_dir = (page_directory_t *) &boot_page_directory + 0xC0000000;
  current_dir = kernel_dir;

  /* Trigger page fault... */
  /* u32 *ptr = (u32 *) 0xdeadbeef; */
  /* (void) ptr; */
  /* *ptr = 10; */

  /* TODO: register interrupt handler for Page Faults */
}

void switch_page_directory(page_directory_t *dir) {
  /* FIXME: Do this in assembly */
  current_dir = dir;
  asm volatile("mov %0, %%cr3":: "r"(&dir->tables_physical));
}

page_t *get_page(u32 addr, u32 make, page_directory_t *dir) {
  addr /= PAGE_SIZE;
  u32 table_index = addr / 1024;

  if (dir->tables[table_index]) {
    return &dir->tables[table_index]->pages[addr % 1024];
  }
  
  if (make) {
    u32 tmp;
    dir->tables[table_index] = (page_table_t *) kmalloc_ap(sizeof(page_table_t), &tmp);
    dir->tables_physical[table_index] = tmp | 0x7; /* Set PRESENT, RW, US */
    return &dir->tables[table_index]->pages[addr % 1024];
  }

  return 0;
}


#include "paging.h"
#include "kheap.h"
#include "util.h"
#include "framebuffer.h"

page_directory_t *kernel_dir;
page_directory_t *current_dir;

/* Bitmap of page frames */
u32 *frames;
u32 nframes;

#define FRAME_INDEX(a) (a/(32))
#define FRAME_OFFSET(a) (a%(32))

void * virt_to_phys(void *addr) {
  u32 pd_index = (u32) addr >> 22;
  u32 pt_index = (u32) addr >> 12 & 0x3FF;
  u32 p_index = (u32) addr & 0xFFF;

  /* u32 *pd = (u32 *) current_dir; */
  u32 *pt = (u32 *) 0xFFC00000 + PAGE_SIZE_BYTES * pd_index;

  return (void *) ((pt[pt_index] & ~0xFFF) + p_index);
}

void set_frame(u32 addr) {
  u32 frame = addr / PAGE_SIZE;
  u32 index = FRAME_INDEX(frame);
  u32 offset = FRAME_OFFSET(frame);
  frames[index] |= ~(0x1 << offset);
}

void clear_frame(u32 addr) {
  u32 frame = addr / PAGE_SIZE;
  u32 index = FRAME_INDEX(frame);
  u32 offset = FRAME_OFFSET(frame);
  frames[index] &= ~(0x1 << offset);
}

u32 check_frame(u32 addr) {
  u32 frame = addr / PAGE_SIZE;
  u32 index = FRAME_INDEX(frame);
  u32 offset = FRAME_OFFSET(frame);
  return (frames[index] & (0x1 << offset));
}

u32 next_available_frame() {
  u32 i, j;
  for (i = 0; i < FRAME_INDEX(nframes); i++) {
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

void alloc_frame(page_t *page, u32 is_kernel, u32 is_writeable) {
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
  /* Assume 128MB of physical memory for now */
  u32 mem_end_page = 0x8000000;
  
  nframes = mem_end_page / PAGE_SIZE;
  frames = (u32 *) kmalloc(FRAME_INDEX(nframes));
  memset(frames, 0, FRAME_INDEX(nframes));

  kernel_dir = (page_directory_t *) &boot_page_directory;
  current_dir = kernel_dir;

  /* Trigger page fault... */
  /* u32 *ptr = (u32 *) 0xdeadbeef; */
  /* (void) ptr; */
  /* *ptr = 10; */

  /* TODO: register interrupt handler for Page Faults */


  char buf[16];
  u32 ptr = (u32) 0x00801004;

  alloc_frame(get_page(ptr, 1, kernel_dir), 1, 1);

  u32 res = (u32) virt_to_phys((void *) ptr);
  itoa((u32) res, buf, 16);
  fb_write(buf);
  fb_write("\n");


  fb_write("Printing kernel page directory:");

  u32 *pde = (u32 *) kernel_dir;
  for (u32 i = 0; i < 1024; i++) {
    if (i % 64 == 0)
      fb_write("\n");

    if (*pde & 0x1) {   /* Page dir entry present - mark frames present */
      fb_write("x");

      /* Mark all frames corresponding to pde as present */
      for (u32 j = 0; j < FRAME_INDEX(1024); j++) {
        frames[FRAME_INDEX(1024) * i + j] = 0xFFFFFFFF;
      }
    } else {            /* Page dir entry empty */
      fb_write(".");
    }

    pde++;
  }

  /* char buff[16]; */
  /* for (u32 i = 0; i < FRAME_INDEX(nframes); i++) { */
    /* fb_write("Frame "); */
    /* itoa(i, buff, 10); */
    /* fb_write(buff); */
    /* fb_write(": "); */
    /* itoa(frames[i], buff, 16); */
    /* fb_write(buff); */
    /* fb_write("\n"); */
  /* } */

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


#ifndef INCLUDE_GDT_H
#define INCLUDE_GDT_H

struct gdt {
  unsigned int address;
  unsigned short size;
} __attribute__((packed));

/** set_gdt
 *  Sets the GDT. Defined in gdt.s  
 *
 *  @param gdt struct defining address and size of segment
 */
void set_gdt(struct gdt table);

#endif /* INCLUDE_GDT_H */

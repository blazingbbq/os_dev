OBJECTS = loader.o \
					kmain.o \
					framebuffer.o \
					serial.o \
					io.o \
					gdt.o \
					idt.o \
					isr.o \
					keyboard.o \
					paging.o \
					kheap.o \
					util.o

CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
			-ffreestanding -fpic -fomit-frame-pointer -nostartfiles \
			-nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

# Create module binary
# TODO: Build these from modules/*
program.bin: program.s
	$(AS) -f bin $< -o $@

os.iso: kernel.elf program.bin
	mkdir -p iso/modules
	cp kernel.elf iso/boot/kernel.elf
	cp program.bin iso/modules
	genisoimage -R                        \
				-b boot/grub/stage2_eltorito    \
				-no-emul-boot                   \
				-boot-load-size 4               \
				-A os                           \
				-input-charset utf8             \
				-quiet                          \
				-boot-info-table                \
				-o os.iso                       \
				iso

run: os.iso
	bochs -f bochsrc.txt -q

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf \
		*.o \
		*.bin \
		kernel.elf \
		os.iso \
		iso/modules

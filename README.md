## Launch the os

### Make

```
Make run
```

### Manual

Generate loader.o: `nasm -f elf32 loader.s`

Link kernel.elf executable: `ld -T link.ld -melf_i386 loader.o -o kernel.elf`

Generate iso image:
```
genisoimage -R                              \
            -b boot/grub/stage2_eltorito    \
            -no-emul-boot                   \
            -boot-load-size 4               \
            -A os                           \
            -input-charset utf8             \
            -quiet                          \
            -boot-info-table                \
            -o os.iso                       \
            iso
```

Run Bochs: `bochs -f bochsrc.txt -q`

## Development requirements

Install dependencies:

```
sudo apt install build-essential nasm genisoimage bochs bochs-sdl bochs-x
```

OS is built for the i386 architecture.


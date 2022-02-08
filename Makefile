all: boot.iso

KSRC := $(wildcard src/*.c)
KOBJ := $(KSRC:.c=.o)

BSRC := $(wildcard boot/*.c)
BOBJ := $(BSRC:.c=.o)
BASMSRC := $(wildcard boot/*.asm)
BASMOBJ := $(BASMSRC:.asm=.o)

CFLAGS := -fno-builtin -ffreestanding -no-pie -fno-pic -mgeneral-regs-only \
	  -mno-red-zone -O3 -W -Wall -z,max-page-size=4096 -nodefaultlibs  \
	  -ggdb -nostdlib

boot.iso: kernel.bin boot/boot.bin
	mkdir -p fs
	mkisofs \
		-o $@ -b boot/grub/stage2_eltorito \
		-r -no-emul-boot -boot-load-size 4 \
		-J -boot-info-table -graft-points \
		boot/boot.bin=boot/boot.bin \
		boot/kernel.bin=kernel.bin \
		fs

kernel.bin: $(KOBJ)
	ld -o $@ $^ -Tsrc/linker.ld -Map kernel.map

boot/boot.bin: $(BOBJ) $(BASMOBJ)
	ld $(BOBJ) $(BASMOBJ) -melf_i386 -T boot/linker.ld -o $@

src/%.o : src/%.c
	$(CC) -c -o $@ $^ $(CFLAGS)

boot/%.o : boot/%.asm
	nasm $^ -o $@ -felf32

boot/%.o : boot/%.c
	$(CC) -c -o $@ $^ $(CFLAGS) -m32

clean:
	$(RM) kernel.bin boot/boot.bin boot.iso $(KOBJ) $(BOBJ)

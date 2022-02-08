KSRC = $(wildcard *.c)
KOBJ = $(KSRC:.c=.o)
KASMSRC = $(wildcard *.asm)
KASMOBJ = $(KASMSRC:.asm=.o)

BSRC = $(wildcard *.c)
BOBJ = $(BSRC:.c=.o)
BASMSRC = $(wildcard *.c)
BASMOBJ = $(BASMSRC:.asm=.o)

CFLAGS = -fno-builtin -ffreestanding -O2 -no-pie -fno-pic -mno-sse

%.o : %.c
	$(CC) $^ -c -o $@ $(CFLAGS) -m32

%.o : %.asm
	nasm -felf32 $^ -o $@

boot.bin: $(ASMOBJ) $(OBJ)
	ld $(OBJ) $(ASMOBJ) -melf_i386 -Tlinker.ld -o boot.bin

clean:
	@rm *.o boot.bin

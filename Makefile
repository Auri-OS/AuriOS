TARGET = AuriOS.bin
ISO = AuriOS.iso
CC = gcc
AS = nasm
LD = ld
CFLAGS = -ffreestanding -O2 -Wall -Wextra -Iincludes
LDFLAGS = -T src/boot/link.ld -nostdlib

OBJS = \
	src/bootloader/loader.o \
	src/memory/memory.o

all: $(TARGET)

src/bootloader/loader.o: src/boot/loader.s
	$(AS) -f elf64 $< -o $@

src/memory/memory.o: src/memory/memory.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

iso: $(TARGET)
	mkdir -p iso/boot/grub
	cp $(TARGET) iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "AuriOS" { multiboot /boot/$(TARGET) }' >> iso/boot/grub/grub.cfg
	grub2-mkrescue -o $(ISO) iso

run: iso
	qemu-system-x86_64 -cdrom $(ISO) 

clean:
	rm -rf $(TARGET) $(ISO) $(OBJS) iso

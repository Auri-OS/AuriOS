TARGET = AuriOS.bin
ISO = AuriOS.iso

CC = i686-elf-gcc
AS = nasm
LD = i686-elf-ld

CFLAGS = -ffreestanding -O2 -Wall -Wextra -m32 -Iincludes
LDFLAGS = -T src/boot/link.ld -nostdlib

OBJS = \
	src/boot/loader.o \
	src/kernel/kernel.o \
	src/memory/memory.o

all: $(TARGET)

# compile loader (32-bit)
src/boot/loader.o: src/boot/loader.s
	$(AS) -f elf32 $< -o $@

# compile kernel file 
src/kernel/kernel.o: src/kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile memory file
src/memory/memory.o: src/memory/memory.c
	$(CC) $(CFLAGS) -c $< -o $@

# Linking final
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# creating ISO file
iso: $(TARGET)
	mkdir -p iso/boot/grub
	cp $(TARGET) iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "AuriOS" { multiboot /boot/$(TARGET) }' >> iso/boot/grub/grub.cfg
	grub2-mkrescue -o $(ISO) iso

# start QEMU
run: iso
	qemu-system-i686 -cdrom $(ISO)

# clean
clean:
	rm -rf $(TARGET) $(ISO) $(OBJS) iso

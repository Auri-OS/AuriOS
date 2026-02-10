TARGET = AuriOS.bin
ISO = AuriOS.iso

CC = i686-elf-gcc
AS = nasm
LD = i686-elf-ld

CFLAGS = -ffreestanding -O2 -Wall -Wextra -m32 -Isrc/includes
LDFLAGS = -T src/boot/link.ld -nostdlib

OBJS = \
    src/boot/loader.o \
    src/memory/memory.o \
    src/memory/GDT.o \
    src/memory/gdt_flush.o \
    src/kernel/kernel.o


# Default target - show help menu
.DEFAULT_GOAL := help

help:
	@echo "=== AuriOS Makefile configuration ==="
	@echo ""
	@echo "Available targets:"
	@echo "  make help           - Show this help menu"
	@echo "  make iso            - Build OS binary and create bootable ISO"
	@echo "  make run            - Build and run in QEMU (x86_64)"
	@echo "  make run32          - Build and run in QEMU (i386)"
	@echo "  make clean          - Remove all build artifacts"
	@echo ""
	@echo "Installation (requires sudo):"
	@echo "  make install-fedora - Install dependencies for Fedora"
	@echo "  make install-arch   - Install dependencies for Arch Linux"
	@echo "  make install-debian - Install dependencies for Debian/Ubuntu"
	@echo ""

# compile loader (32-bit)
src/boot/loader.o: src/boot/loader.s
	$(AS) -f elf32 $< -o $@

# compile kernel file 
src/kernel/kernel.o: src/kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile memory file
src/memory/memory.o: src/memory/memory.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile GDT file
src/memory/GDT.o: src/memory/GDT.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile GDT flush assembly file
src/memory/gdt_flush.o: src/memory/gdt_flush.asm
	$(AS) -f elf32 $< -o $@

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
	qemu-system-x86_64 -cdrom $(ISO) -m 512M -boot d -vga std

#start QEMU x32
run32: iso
	qemu-system-i386 -cdrom $(ISO) -m 512M -boot d -vga std 

# clean
clean:
	rm -rf $(TARGET) $(ISO) $(OBJS) iso

install-fedora:
	echo "[!] Installing dependencies for Fedora"
	sudo dnf install gcc gcc-c++ binutils make wget tar texinfo gmp-devel mpfr-devel libmpc-devel nasm qemu-system-x86 grub2-tools-extra mtools xorriso
	bash docs/install_scripts/install.sh
	
install-arch:
	echo "[!] Installing dependencies for Arch Linux"
	sudo pacman -S gcc binutils make wget tar nasm qemu-system-x86 grub mtools xorriso
	bash docs/install_scripts/install.sh

install-debian:
	echo "[!] Installing dependencies for Debian/Ubuntu"
	sudo apt install gcc g++ binutils make wget tar mtools xorriso nasm qemu-system-x86 grub-pc-bin
	bash docs/install_scripts/install.sh
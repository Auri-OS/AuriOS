# AuriOS Makefile
# Output directory for final binaries
OUTPUT_DIR = output
BUILD_DIR = build
ISO_DIR = iso

# Final binaries
KERNEL_BIN = $(OUTPUT_DIR)/AuriOS.bin
ISO = $(OUTPUT_DIR)/AuriOS.iso

# Toolchain
CC = i686-elf-gcc
AS = nasm
LD = i686-elf-ld

# Flags
CFLAGS = -ffreestanding -O2 -Wall -Wextra -m32 -Isrc/include
LDFLAGS = -T linker.ld -nostdlib

# Source files
C_SOURCES = $(wildcard src/kernel/*.c) $(wildcard src/cpu/*.c) $(wildcard src/lib/*.c) $(wildcard src/drivers/*.c)
S_SOURCES = $(wildcard src/boot/*.s)
ASM_SOURCES = $(wildcard src/cpu/*.asm)

# Object files (in build directory)
C_OBJS = $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
S_OBJS = $(patsubst src/%.s, $(BUILD_DIR)/%.o, $(S_SOURCES))
ASM_OBJS = $(patsubst src/%.asm, $(BUILD_DIR)/%.o, $(ASM_SOURCES))

OBJS = $(S_OBJS) $(ASM_OBJS) $(C_OBJS)

# Default target
.DEFAULT_GOAL := help

# Phony targets
.PHONY: all clean help iso run run32 install-fedora install-arch install-debian

help:
	@echo "======================= AuriOS Makefile ======================="
	@echo ""
	@echo "Available targets:"
	@echo "  make all            - Build everything"
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
	@echo "==============================================================="

# Create necessary directories
$(BUILD_DIR) $(OUTPUT_DIR):
	@mkdir -p $@
	@mkdir -p $(BUILD_DIR)/boot
	@mkdir -p $(BUILD_DIR)/kernel
	@mkdir -p $(BUILD_DIR)/cpu
	@mkdir -p $(BUILD_DIR)/lib

# Compile C source files
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile assembly files (.s)
$(BUILD_DIR)/%.o: src/%.s | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "AS $<"
	@$(AS) -f elf32 $< -o $@

# Compile assembly files (.asm)
$(BUILD_DIR)/%.o: src/%.asm | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "AS $<"
	@$(AS) -f elf32 $< -o $@

# Link kernel binary
$(KERNEL_BIN): $(OBJS) | $(OUTPUT_DIR)
	@echo "LD $(KERNEL_BIN)"
	@$(LD) $(LDFLAGS) -o $@ $(OBJS)
	@echo "Build complete: $(KERNEL_BIN)"

# Build all
all: $(KERNEL_BIN)

# Create bootable ISO
iso: $(KERNEL_BIN)
	@echo "Creating ISO..."
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(KERNEL_BIN) $(ISO_DIR)/boot/
	@echo 'set timeout=0' > $(ISO_DIR)/boot/grub/grub.cfg
	@echo 'set default=0' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo 'menuentry "AuriOS" {' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '    multiboot /boot/AuriOS.bin' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '    boot' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '}' >> $(ISO_DIR)/boot/grub/grub.cfg
	@grub2-mkrescue -o $(ISO) $(ISO_DIR) 2>/dev/null || grub-mkrescue -o $(ISO) $(ISO_DIR)
	@echo "ISO created: $(ISO)"

# Run in QEMU (x86_64)
run: iso
	@echo "Starting QEMU (x86_64)..."
	@qemu-system-x86_64 -cdrom $(ISO) -m 512M -boot d -vga std

# Run in QEMU (i386)
run32: iso
	@echo "Starting QEMU (i386)..."
	@qemu-system-i386 -cdrom $(ISO) -m 512M -boot d -vga std

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR) $(OUTPUT_DIR) $(ISO_DIR)
	@echo "Clean complete."

# Installation targets
install-fedora:
	@echo "[!] Installing dependencies for Fedora"
	sudo dnf install gcc gcc-c++ binutils make wget tar texinfo gmp-devel mpfr-devel libmpc-devel nasm qemu-system-x86 grub2-tools-extra mtools xorriso
	bash docs/install_scripts/install.sh
	
install-arch:
	@echo "[!] Installing dependencies for Arch Linux"
	sudo pacman -S gcc binutils make wget tar nasm qemu-system-x86 grub mtools xorriso
	bash docs/install_scripts/install.sh

install-debian:
	@echo "[!] Installing dependencies for Debian/Ubuntu"
	sudo apt install gcc g++ binutils make wget tar mtools xorriso nasm qemu-system-x86 grub-pc-bin
	bash docs/install_scripts/install.sh
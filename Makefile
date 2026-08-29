# AuriOS Makefile
# Output directory for final binaries
OUTPUT_DIR = output
BUILD_DIR = build
HEADER_DIR = src/include
ISO_DIR = iso

# Final binaries
KERNEL_BIN = $(OUTPUT_DIR)/AuriOS.bin
ISO = $(OUTPUT_DIR)/AuriOS.iso

# ==========================================
# Toolchain Configuration
# Usage: make              (uses GCC by default)
#        make USE_ZIG=1    (uses Zig toolchain)
# ==========================================
USE_ZIG ?= 0

ifeq ($(USE_ZIG), 1)
    CC = zig cc -target x86-freestanding-none
    LD = zig ld.lld
    CFLAGS = -ffreestanding -Wall -Wextra -m32 -I src/include -fno-pie -fno-stack-protector -mgeneral-regs-only -fno-sanitize=all 
    LDFLAGS = -T linker.ld -nostdlib -z max-page-size=0x1000 --build-id=none
else
    # Check if i686-elf-gcc is available, otherwise fall back to host gcc
    ifeq ($(shell command -v i686-elf-gcc > /dev/null 2>&1 && echo 1 || echo 0), 1)
        CC = i686-elf-gcc
        LD = i686-elf-ld
    else
        CC = gcc
        LD = ld
    endif
    CFLAGS = -ffreestanding -O2 -Wall -Wextra -m32 -Isrc/include
    LDFLAGS = -T linker.ld -nostdlib -m elf_i386
endif


AS = nasm

# Source files
C_SOURCES = $(wildcard src/kernel/*.c) $(wildcard src/cpu/*.c) $(wildcard src/lib/*.c) $(wildcard src/drivers/*.c)
S_SOURCES = $(wildcard src/boot/*.s)
ASM_SOURCES = $(wildcard src/cpu/*.asm)
ZIG_SOURCES = $(wildcard src/kernel/*.zig) $(wildcard src/mm/*.zig)

# Object files (in build directory)
C_OBJS = $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
S_OBJS = $(patsubst src/%.s, $(BUILD_DIR)/%.o, $(S_SOURCES))
ASM_OBJS = $(patsubst src/%.asm, $(BUILD_DIR)/%.o, $(ASM_SOURCES))
ZIG_OBJS = $(patsubst src/%.zig, $(BUILD_DIR)/%.o, $(ZIG_SOURCES))

OBJS = $(S_OBJS) $(ASM_OBJS) $(C_OBJS) $(ZIG_OBJS)

# Default target
.DEFAULT_GOAL := help

# Phony targets
.PHONY: all clean help iso iso-debug run run32 run-mac install-fedora install-arch install-debian

help:
	@echo "======================= AuriOS Makefile ======================="
	@echo ""
	@echo "Installation (requires admin rights):"
	@echo "  make install - Install dependencies (cross-platform)"
	@echo ""
	@echo "Compilation targets:"
	@echo "  make all            - Build everything"
	@echo "  make iso            - Build OS binary and create bootable ISO"
	@echo "  make iso-debug      - Build bootable ISO with Test Mode enabled (serial output)"
	@echo ""
	@echo "Execution targets:"
	@echo "  make run            - Build and run in QEMU (x86_64)"
	@echo "  make clean          - Remove all build artifacts"
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

# Compile zig files (.zig)
$(BUILD_DIR)/%.o: src/%.zig | $(BUILD_DIR) 
	@mkdir -p $(dir $@)
	@echo "ZIG $<"
	@zig build-obj $< -femit-bin=$@ -target x86-freestanding-none -O ReleaseSafe -fno-stack-check -mcpu=i386 -I $(HEADER_DIR)

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
	@echo 'terminal_input console' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo 'terminal_output console' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo 'menuentry "AuriOS" {' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '    set gfxpayload=text' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '    multiboot /boot/AuriOS.bin' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '    boot' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '}' >> $(ISO_DIR)/boot/grub/grub.cfg
	@grub-mkrescue -o $(ISO) $(ISO_DIR) 2>/dev/null || grub2-mkrescue -o $(ISO) $(ISO_DIR)
	@echo "ISO created: $(ISO)"

iso-debug:
	@echo "Building Test ISO with AURI_TEST_MODE..."
	@$(MAKE) clean
	@$(MAKE) CFLAGS="$(CFLAGS) -DAURI_TEST_MODE" iso
	@echo "Test ISO build complete!"

# Run in QEMU (auto-detect Linux/macOS)
run: iso
	@if [ "$$(uname)" = "Darwin" ]; then \
		echo "Starting QEMU on macOS (direct kernel boot)..."; \
		qemu-system-i386 -kernel $(KERNEL_BIN) -m 512M -vga std -serial stdio -display cocoa,zoom-to-fit=on; \
	else \
		echo "Starting QEMU on Linux (x86_64)..."; \
		qemu-system-x86_64 -cdrom $(ISO) -m 512M -boot d -vga std -serial stdio; \
	fi

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR) $(OUTPUT_DIR) $(ISO_DIR)
	@echo "Clean complete."

# Installation target
install:
	@echo "[!] Installing dependencies for your OS..."
	@if [ "$$(uname)" = "Darwin" ]; then \
		brew install qemu i686-elf-gcc nasm zig clang-format; \
	elif [ -f /etc/arch-release ]; then \
		sudo pacman -S --noconfirm gcc binutils make wget tar mtools xorriso nasm qemu-system-x86 grub clang zig; \
		bash docs/install_scripts/install.sh; \
	elif [ -f /etc/fedora-release ]; then \
		sudo dnf install -y gcc gcc-c++ binutils make wget tar texinfo gmp-devel mpfr-devel libmpc-devel nasm qemu-system-x86 grub2-tools-extra mtools xorriso clang-tools-extra zig; \
		bash docs/install_scripts/install.sh; \
	elif [ -f /etc/debian_version ]; then \
		sudo apt install -y gcc g++ binutils make wget tar mtools xorriso nasm qemu-system-x86 grub-pc-bin clang-format zig; \
		bash docs/install_scripts/install.sh; \
	else \
		echo "Unsupported OS for auto-install. Please visit https://auri-os.org/docs"; \
	fi

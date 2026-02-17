<div align="center">

# 🌟 AuriOS

```
        X       
       XXX      
      XXXXX     
     X XXXXX    
    XXX XXXXX   
   XXXXX XXXXX   
  XXXXXX  XXXXX  
 XXXXXX    XXXXX 
XXXXXX      XXXXX
```

**A minimal x86 operating system kernel written in C and Assembly**

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Version](https://img.shields.io/badge/version-0.2.0-green.svg)](https://github.com/Auri-OS/AuriOS/releases)
[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)]()

[Features](#-features) • [Getting Started](#-getting-started) • [Installation](#-installation) • [Usage](#-usage) • [Documentation](#-documentation) • [Contributing](#-contributing)

</div>

---

## 📖 About

**AuriOS** is a simple, educational operating system kernel built from scratch for x86 architecture. It's designed to help understand the fundamentals of operating system development, including memory management, interrupt handling, and hardware interaction.

This project demonstrates low-level system programming concepts and serves as a learning platform for OS development enthusiasts.

## ✨ Features

- **🔧 Core System Components**
  - Global Descriptor Table (GDT) initialization
  - Interrupt Descriptor Table (IDT) with ISR handlers
  - Programmable Interrupt Controller (PIC) remapping
  - Custom bootloader

- **💾 Memory Management**
  - Basic memory initialization
  - Dynamic memory allocation (malloc)
  - Memory utility functions

- **⚙️ Hardware Drivers**
  - Keyboard driver with input handling
  - Programmable Interval Timer (PIT)
  - VGA text mode terminal

- **🖥️ User Interface**
  - Interactive shell
  - Terminal with color support
  - Command-line interface

- **🛠️ Development Tools**
  - Comprehensive Makefile
  - QEMU integration for testing
  - ISO generation for bootable media

## 🚀 Getting Started

### Prerequisites

Before building AuriOS, ensure you have the following tools installed:

- **Cross-compiler**: `i686-elf-gcc` (GCC configured for i686-elf target)
- **Assembler**: `nasm` (Netwide Assembler)
- **Linker**: `i686-elf-ld`
- **Emulator**: `qemu-system-i386` or `qemu-system-x86_64`
- **Build tools**: `make`, `grub-mkrescue` (for ISO creation)
- **Additional**: `xorriso`, `mtools` (for ISO generation)

### Quick Install Dependencies

#### Fedora / RHEL
```bash
make install-fedora
```

#### Arch Linux
```bash
make install-arch
```

#### Debian / Ubuntu
```bash
make install-debian
```

#### macOS
```bash
brew install i686-elf-gcc nasm qemu xorriso
```

> **Note**: For detailed installation instructions for your platform, see [docs/INSTALLATION.MD](docs/INSTALLATION.MD)

## 🔨 Installation

### 1. Clone the Repository

```bash
git clone https://github.com/Auri-OS/AuriOS.git
cd AuriOS
```

### 2. Build the Kernel

```bash
make all
```

This will:
- Compile all C source files
- Assemble all assembly files
- Link everything into a kernel binary
- Generate a bootable ISO image

### 3. Run in QEMU

```bash
make run        # For x86_64 QEMU
make run32      # For i386 QEMU
make run-mac    # For macOS (direct boot)
```

## 📚 Usage

Once AuriOS boots, you'll see the initialization sequence followed by an interactive shell. The shell supports basic commands and keyboard input.

### Available Make Targets

```bash
make all            # Build everything
make iso            # Generate bootable ISO
make run            # Build and run in QEMU (x86_64)
make run32          # Build and run in QEMU (i386)
make run-mac        # Build and run on macOS
make clean          # Remove all build artifacts
make help           # Show all available targets
```

## 📁 Project Structure

```
AuriOS/
├── src/
│   ├── boot/           # Bootloader code
│   │   └── loader.s    # Assembly bootloader
│   ├── cpu/            # CPU-related code
│   │   ├── gdt.c       # Global Descriptor Table
│   │   ├── idt.c       # Interrupt Descriptor Table
│   │   ├── isr.c       # Interrupt Service Routines
│   │   ├── irq.c       # IRQ handlers
│   │   └── pic.c       # PIC configuration
│   ├── drivers/        # Hardware drivers
│   │   ├── keyboard.c  # Keyboard driver
│   │   └── timer.c     # Timer driver
│   ├── kernel/         # Kernel core
│   │   ├── kernel.c    # Main kernel entry
│   │   ├── shell.c     # Interactive shell
│   │   └── terminal.c  # Terminal output
│   ├── lib/            # Standard library functions
│   │   ├── malloc.c    # Memory allocation
│   │   ├── memory.c    # Memory utilities
│   │   └── string.c    # String operations
│   └── include/        # Header files
├── build/              # Build artifacts
├── output/             # Final binaries and ISO
├── docs/               # Documentation
├── linker.ld           # Linker script
└── Makefile            # Build configuration
```

## 📖 Documentation

- [Installation Guide](docs/INSTALLATION.MD) - Detailed setup instructions
- [Bootloader Documentation](docs/loader.md) - How the bootloader works
- [Contributing Guidelines](docs/CONTRIBUTING.md) - How to contribute
- [Setup & Installation](docs/setup-installation.md) - Development environment setup

## 🤝 Contributing

Contributions are welcome! Whether you're fixing bugs, adding features, or improving documentation, your help is appreciated.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

Please read [CONTRIBUTING.md](docs/CONTRIBUTING.md) for details on our code of conduct and development process.

## 🗺️ Roadmap

### Current Version (v0.2.0)
- ✅ Basic kernel initialization
- ✅ GDT and IDT setup
- ✅ Keyboard driver
- ✅ Timer driver
- ✅ Interactive shell
- ✅ Memory management basics

### Planned Features
- [ ] Virtual memory management
- [ ] File system support (FAT32)
- [ ] Multi-tasking and process scheduling
- [ ] System calls interface
- [ ] Extended driver support (ATA, VFS)
- [ ] More shell commands
- [ ] GUI framework

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- The OSDev community for extensive documentation
- The creators of GRUB for the bootloader
- All contributors who have helped improve AuriOS

## 📧 Contact

Project Link: [https://github.com/Auri-OS/AuriOS](https://github.com/Auri-OS/AuriOS)

---

<div align="center">

**Made with ❤️ by the AuriOS Team**

⭐ Star this repository if you find it helpful!

</div>
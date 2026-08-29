<div align="center">

# 🌟 AuriOS

```
          .**.
         .=###.
        .==.##%
       .===.###
      .=====###.
     .======.###
    .======..###.
   .===.     ###.
             .***
```

**A minimal x86 operating system kernel written in C and Assembly**

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Version](https://img.shields.io/badge/version-0.2.1-aqua.svg)](https://github.com/Auri-OS/AuriOS/releases)
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

## ❤️ Contributors

Thank to all the contributors, you are the flame burning in our heart ❤️

<div align="left">

|                                           Profile                                           |                     GitHub                      |          Role           |
| :-----------------------------------------------------------------------------------------: | :---------------------------------------------: | :---------------------: |
|     <img src="https://github.com/im-nymii.png" width="80" height="80" alt="im-nymii" />     |      [nymii](https://github.com/im-nymii)       |  Founder & Maintainer   |
|    <img src="https://github.com/llmaddie.png" width="80" height="80" alt="llmaddie 2" />    |      [Maddie](https://github.com/llmaddie)      | Co-Founder & Maintainer |
|    <img src="https://github.com/pepedinho.png" width="80" height="80" alt="pepedinho" />    |    [pepedinho](https://github.com/pepedinho)    |      Co-Maintainer      |
|  <img src="https://github.com/swtchcoder.png" width="80" height="80" alt="switchcodeur" />  |  [switchcodeur](https://github.com/swtchcoder)  |       Contributor       |
|       <img src="https://github.com/proxzr.png" width="80" height="80" alt="proxzr" />       |       [proxzr](https://github.com/proxzr)       |       Contributor       |
|      <img src="https://github.com/aomitsu.png" width="80" height="80" alt="Aomitsu" />      |      [Aomitsu](https://github.com/aomitsu)      |       Contributor       |
|       <img src="https://github.com/ivy-js.png" width="80" height="80" alt="ivy-js" />       |       [Ivy-js](https://github.com/ivy-js)       |       Contributor       |
| <img src="https://github.com/gittihub-jpg.png" width="80" height="80" alt="gittihub-jpg" /> | [gittihub-jpg](https://github.com/gittihub-jpg) |       Contributor       |
| <img src="https://github.com/jesuiskoriel.png" width="80" height="80" alt="jesuiskoriel" /> | [Jesuiskoriel](https://github.com/Jesuiskoriel) |       Contributor       |
| <img src="https://github.com/kudasaixc.png" width="80" height="80" alt="kudasaixc" /> | [Kudasai](https://github.com/kudasaixc) |   Contributor & Bug Hunter    |
| <img src="https://github.com/frenchcast1234.png" width="80" height="80" alt="frenchcast1234"> | [frenchcast1234](https://github.com/frenchcast1234) | Contributor |

</div>

## 🚀 Getting Started

### Prerequisites

Before building AuriOS, ensure you have the following tools installed:

- **Cross-compiler**: `i686-elf-gcc` (GCC configured for i686-elf target)
- **Assembler**: `nasm` (Netwide Assembler)
- **Linker**: `i686-elf-ld`
- **Emulator**: `qemu-system-i386` for arm - or `qemu-system-x86_64` for x86
- **Build tools**: `make`, `grub-mkrescue` (for ISO creation)
- **Additional**: `xorriso`, `mtools` (for ISO generation)

## 🔨 Installation

### 1. Clone the Repository

```bash
git clone https://github.com/Auri-OS/AuriOS.git
cd AuriOS
```

### 2. Quick Install Dependencies

#### Cross Platform install :

```bash
make install
```

> **Note**: For detailed installation instructions for your platform, see [Requirements & environment](https://auri-os.org/docs/installation/) from the documentation.


### 3. Build the Kernel

```bash
make all
```

This will:

- Compile all C source files
- Assemble all assembly files
- Link everything into a kernel binary
- Generate a bootable ISO image

### 4. Run in QEMU

```bash
make run
```
### Available Make Targets

```bash
❯ make
======================= AuriOS Makefile =======================

Installation (requires admin rights):
  make install - Install dependencies (cross-platform)

Compilation targets:
  make all            - Build everything
  make iso            - Build OS binary and create bootable ISO
  make iso-debug      - Build bootable ISO with Test Mode enabled (serial output)

Execution targets:
  make run            - Build and run in QEMU (x86_64)
  make clean          - Remove all build artifacts

===============================================================
```
## 📚 Usage

Once AuriOS boots, you'll see the initialization sequence followed by an interactive shell. The shell supports basic commands and keyboard input.


## 📁 Project Structure

```
AuriOS/
├── scripts/
│   └── install.sh
├── src/
│   ├── boot/
│   │   └── loader.s
│   ├── cpu/
│   │   ├── gdt.c
│   │   ├── gdt_flush.asm
│   │   ├── idt.c
│   │   ├── idt_flush.asm
│   │   ├── irq.c
│   │   ├── isr.c
│   │   ├── isr_stubs.asm
│   │   └── pic.c
│   ├── drivers/
│   │   ├── framebuffer.c
│   │   ├── keyboard.c
│   │   ├── serial.c
│   │   └── timer.c
│   ├── include/
│   │   ├── ansi.h
│   │   ├── colors.h
│   │   ├── fetch.h
│   │   ├── font.h
│   │   ├── framebuffer.h
│   │   ├── gdt.h
│   │   ├── history.h
│   │   ├── idt.h
│   │   ├── integer.h
│   │   ├── io.h
│   │   ├── isr.h
│   │   ├── keyboard.h
│   │   ├── log.h
│   │   ├── memory.h
│   │   ├── mm.h
│   │   ├── multiboot.h
│   │   ├── pic.h
│   │   ├── serial.h
│   │   ├── shell.h
│   │   ├── string.h
│   │   ├── terminal.h
│   │   ├── timer.h
│   │   └── types.h
│   ├── kernel/
│   │   ├── ainsi.zig
│   │   ├── history.c
│   │   ├── kernel.c
│   │   ├── log.c
│   │   ├── shell.c
│   │   └── terminal.c
│   ├── lib/
│   │   ├── integer.c
│   │   ├── memory.c
│   │   └── string.c
│   └── mm/
│       ├── mmu.zig
│       └── pmm.zig
├── tests/
│   └── integrations
│       ├── error_handling.yml
│       ├── memory_tools.yml
│       ├── shell.yml
│       ├── timer_flags.yml
│       └── walkman.yaml
├── LICENSE
├── linker.ld
├── Makefile
├── README.md
└── walkman.yaml
```

## 📖 Documentation

Check out our full [documentation](https://auri-os.org/docs) !

- [Requirements & environment](https://auri-os.org/docs/installation/) - Detailed setup instructions
- [Contributing Guidelines](https://auri-os.org/docs/contributing/) - How to contribute
- [Building & running](https://auri-os.org/docs/building/) - Development environment setup

## 🤝 Contributing

Contributions are welcome! Whether you're fixing bugs, adding features, or improving documentation, your help is appreciated.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

Please read [Contributing Guidelines](https://auri-os.org/docs/contributing/) for details on our code of conduct and development process.

## 🗺️ Roadmap

### Current Version (v0.2.1)

- [x] Basic kernel initialization
- [x] GDT and IDT setup
- [x] Keyboard driver
- [x] Timer driver
- [x] Interactive shell
- [x] Memory management basics
- [x] More shell commands

### Planned Features

- [ ] Virtual memory management
- [ ] File system support (FAT32)
- [ ] Multi-tasking and process scheduling
- [ ] System calls interface
- [ ] Extended driver support (ATA, VFS)
- [ ] GUI framework

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- The OSDev community for extensive documentation
- The creators of GRUB for the bootloader
- All contributors who have helped improve AuriOS

## 📧 Contact

Project Link: [https://github.com/Auri-OS/AuriOS](https://github.com/Auri-OS/AuriOS)

Discord Server: [AuriOS Discord](https://discord.gg/SckCXYq7G)

X: [@tryAuriOS](https://x.com/tryAuriOS)

Website: [auri-os.org](https://auri-os.org)

Documentation: [auri-os.org/docs](https://auri-os.org/docs)

Taskboard: [@Auri-OS 's Todo-List !](https://github.com/orgs/Auri-OS/projects/1)

---

<div align="center">

**Made with ❤️ by the AuriOS Team**

⭐ Star this repository if you find it helpful!

</div>

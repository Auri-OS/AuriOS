# Contributing to AuriOS

Thank you for your interest in contributing to AuriOS! We welcome contributions from everyone.

## How to Contribute

0. **Install a good development environment**

   We use **Visual Studio Code** with automatic code formatting configured.

   ### Required Setup:

   **Install clang-format** (for C/Assembly code):

   ```bash
   # macOS
   brew install clang-format

   # Linux (Ubuntu/Debian)
   sudo apt install clang-format

   # Linux (Fedora/RHEL)
   sudo dnf install clang-tools-extra

   # Linux (Arch)
   sudo pacman -S clang
   ```

   **Install VS Code Extensions**:
   - [clang-format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format) - Formats C/Assembly code

   ### How it works:
   - When you press **Ctrl+S** (or Cmd+S on macOS), the code is automatically formatted
   - **clang-format** formats `.c` and `.h` files (C code)
   - Configuration file (`.clang-format`) is already in the repo
   - **No additional setup needed** - it just works after installing the extension!

   ### Why we do this:
   - Consistent code style across the whole project
   - No formatting debates in code reviews
   - Clean, readable kernel code
   - Automatic on save = no manual formatting needed

1. **Fork the repository**
   Click the "Fork" button to create your own copy.

2. **Clone your fork**

   ```bash
   git clone https://github.com/your-username/AuriOS.git
   ```

3. **Create a branch from Develop**

   ```bash
   git checkout develop
   git branch -C feature/your-feature-name
   ```

   existing cases : "feat/", "fix/", "ref/".

4. **Make your changes**
   Implement your feature or fix.

5. **Commit your changes**

   ```bash
   git commit -m "feat: implement PMM bitmap (#12)"
   ```

   _(If you are working on an open issue, please systematically mention its number in your commit messages!)_

6. **Push to your fork**

   ```bash
   git push origin feature/your-feature-name
   ```

7. **Open a Pull Request**
   Go to the original repository and open a pull request to the **develop branch**.

8. **Select the correct branch**
   Please do not ask to push on AuriOS:main but on AuriOS:develop.

## Guidelines

- Follow the project's coding style.
- Write clear commit messages.
- Document your code when necessary.
- Ensure your changes do not break existing functionality.

## Code of Conduct

Please be respectful and considerate in all interactions.

---

Thank you for helping make AuriOS better!

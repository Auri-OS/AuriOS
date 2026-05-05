#include "../include/shell.h"
#include "../include/terminal.h"
#include "../include/string.h"
#include "../include/timer.h"
#include "../include/integer.h"

#define BUFFER_SIZE 256
static char buffer[BUFFER_SIZE];
static int buffer_pos = 0;

void shell_init(void) {
    terminal_writestring("kernel@auri-os~$ ");
}

static void shell_execute(char* cmd)
{
    // --- 1. TEKNİK DÜZELTME: Pointer Aritmetiği ile In-Place Trim ---
    // Baştaki boşlukları ve tabları atla (Trim leading spaces)
    while (*cmd == ' ' || *cmd == '\t') {
        cmd++;
    }

    // Eğer komut tamamen boşluklardan oluşuyorsa geri dön
    if (*cmd == '\0') return;

    // Sondaki boşlukları temizle (Trim trailing spaces)
    char* end = cmd;
    while (*end != '\0') {
        end++;
    }
    end--; // \0 karakterinden bir önceki karaktere dön

    // Sondaki boşluk, tab ve satır sonu karakterlerini null (\0) ile değiştir
    while (end > cmd && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }

    // --- 2. KOMUT İŞLEME VE ÖZGÜNLEŞTİRİLMİŞ İÇERİK ---
    if (strcmp(cmd, "help") == 0) {
        terminal_writestring("\n--- AuriOS Help Menu ---\n");
        terminal_writestring("help    - Show this command list\n");
        terminal_writestring("about   - System information and credits\n");
        terminal_writestring("clear   - Clear the terminal screen\n");
        terminal_writestring("uptime  - Show system running time\n");
        terminal_writestring("crash   - Halt the system (for testing)\n\n");
    }
    else if (strcmp(cmd, "clear") == 0) {
        terminal_clear();
    }
    else if (strcmp(cmd, "about") == 0) {
        // --- ÖZGÜNLÜK VE ÇABA: Yenilenmiş Hakkında Sayfası ---
        terminal_writestring("\n          _______  _______  _______ \n");
        terminal_writestring("         |   _   ||   _   ||   _   |\n");
        terminal_writestring("         |  |_|  ||  |_|  ||  |_|  |\n");
        terminal_writestring("         |       ||       ||       |\n");
        terminal_writestring("         |   _   ||   _   ||   _   |\n");
        terminal_writestring("         |  | |  ||  | |  ||  | |  |\n");
        terminal_writestring("         |__| |__||__| |__||__| |__|\n\n");
        terminal_writestring("   AuriOS v0.2.0-STABLE\n");
        terminal_writestring("   ------------------------------------\n");
        terminal_writestring("   Kernel       : AuriKernel (x86_32)\n");
        terminal_writestring("   Architecture : i386\n");
        terminal_writestring("   Maintainer   : Auri-OS Team\n");
        terminal_writestring("   Contributor  : Zekiye Meral\n"); // Senin imzan
        terminal_writestring("   Project      : OSS Final Course Work\n");
        terminal_writestring("   Status       : Active Development\n");
        terminal_writestring("   ------------------------------------\n");
        terminal_writestring("   Type 'help' for available commands.\n\n");
    }
    else if (strcmp(cmd, "crash") == 0) {
        asm volatile("cli");
        for (;;) asm volatile("hlt");
    }
    else if (strcmp(cmd, "uptime") == 0) {
        uint32_t ticks = get_tick();
        uint32_t seconds = ticks / 1000;
        uint32_t minutes = seconds / 60;
        uint32_t hours = minutes / 60;
        
        char buf[32];
        terminal_writestring("System Uptime: ");
        itoa(hours, buf); terminal_writestring(buf); terminal_writestring("h ");
        itoa(minutes, buf); terminal_writestring(buf); terminal_writestring("m ");
        itoa(seconds, buf); terminal_writestring(buf); terminal_writestring("s\n");
    }
    else {
        terminal_writestring("command not found: ");
        terminal_writestring(cmd);
        terminal_putchar('\n');
    }
}

void shell_handle_key(char c)
{
    if (c == 0x0C) {
        terminal_clear();
        shell_init();
        return;
    }
    if (c == '\n') {
        terminal_putchar('\n');
        buffer[buffer_pos] = '\0';
        shell_execute(buffer);
        buffer_pos = 0;
        terminal_writestring("kernel@auri-os~$ ");
    }
    else if (c == '\b') {
        if (buffer_pos > 0) {
            buffer_pos--;
            terminal_backspace();
        }
    }
    else {
        if (buffer_pos < BUFFER_SIZE - 1) {
            buffer[buffer_pos++] = c;
            terminal_putchar(c);
        }
    }
}

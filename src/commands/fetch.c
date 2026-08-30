#include "../include/terminal.h"
#include "../include/colors.h"
#include "../include/fetch.h"
#include "../include/commands.h"

/*
To modify the info strings displayed to the right of the fetch ASCII art,
edit src/include/fetch.h
*/
void fetch(void) {
    terminal_writestring(
        "\n          " COLOR_WHITE_BRIGHT ".**." COLOR_RESET "                 \n");
    ///
    terminal_writestring("         " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT "=" COLOR_BLUE_BRIGHT
                         "###" COLOR_WHITE_BRIGHT "." COLOR_RESET "             " fetch_user "\n");
    ///
    terminal_writestring("        " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT "==" COLOR_WHITE_BRIGHT
                         "." COLOR_BLUE_BRIGHT "##%" COLOR_WHITE_BRIGHT "." COLOR_RESET
                         "            --------------\n");
    ///
    terminal_writestring("       " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT "===" COLOR_WHITE_BRIGHT
                         "." COLOR_BLUE_BRIGHT "###" COLOR_WHITE_BRIGHT "." COLOR_RESET
                         "            " fetch_kernel_name "\n");
    ///
    terminal_writestring(
        "      " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT "=====" COLOR_BLUE_BRIGHT
        "###" COLOR_WHITE_BRIGHT "." COLOR_RESET "            " fetch_version "\n");
    ///
    terminal_writestring("     " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT
                         "======" COLOR_WHITE_BRIGHT "." COLOR_BLUE_BRIGHT "###" COLOR_WHITE_BRIGHT
                         "." COLOR_RESET "           " fetch_release_date "\n");
    ///
    terminal_writestring(
        "    " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT "======" COLOR_WHITE_BRIGHT
        ".." COLOR_BLUE_BRIGHT "###" COLOR_WHITE_BRIGHT "." COLOR_RESET "         \n");
    ///
    terminal_writestring(
        "   " COLOR_WHITE_BRIGHT ".===.    ." COLOR_BLUE_BRIGHT "###" COLOR_WHITE_BRIGHT
        "." COLOR_RESET "           " bg_color_bright "\n");
    ///
    terminal_writestring(
        "             " COLOR_WHITE_BRIGHT ".***." COLOR_RESET "          " bg_color "\n\n");
    ///
    terminal_writestring("Type 'help' for available commands\n\n");
}

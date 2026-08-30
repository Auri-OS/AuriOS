#include "../include/types.h"
#include "../include/timer.h"
#include "../include/terminal.h"
#include "../include/string.h"
#include "../include/commands.h"

void uptime(void *args, int argc) {
    char **argv = (char **)args;
    uint32_t ticks = get_tick();
    uint32_t total_seconds = ticks / 1000;
    uint32_t seconds = total_seconds % 60;

    uint32_t total_minutes = total_seconds / 60;
    uint32_t minutes = total_minutes % 60;

    uint32_t hours = total_minutes / 60;

    int j = 1;
    int raw = 0;
    int sec = 0;
    int pretty = 0;
    while (j < argc && argv[j][0] == '-') {
        if (strcmp(argv[j], "-h") == 0) {
            terminal_writestring("uptime - show uptime since machine started\n");
            terminal_writestring("-h     - show this message\n");
            terminal_writestring("-r     - show uptime in miliseconds\n");
            terminal_writestring("-s     - show uptime in seconds\n");
            terminal_writestring("-p     - show uptime in a pretty format\n");
            return;
        }
        else if (strcmp(argv[j], "-r") == 0)
            raw = 1;
        else if (strcmp(argv[j], "-s") == 0)
            sec = 1;
        else if (strcmp(argv[j], "-p") == 0)
            pretty = 1;
        else {
            break;
        }
        j++;
    }

    if (raw == 1) {
        print_unit(ticks, "ms", 1);
    }
    else if (sec == 1) {
        print_unit(total_seconds, "s", 1);
    }
    else if (pretty == 1) {
        terminal_writestring("Current Uptime: \n");
        if (hours != 0)
            print_unit(hours, hours > 1 ? " hours" : " hour", 1);
        print_unit(minutes, minutes > 1 ? " minutes" : " minute", 1);
        print_unit(seconds, seconds > 1 ? " seconds" : " second", 1);
        terminal_writestring("\n");
    }
    else {
        terminal_writestring("Current Uptime: ");
        print_unit(hours, "h ", 0);
        print_unit(minutes, "m ", 0);
        print_unit(seconds, "s", 1);
    }
}
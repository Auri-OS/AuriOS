#include "history.h"
#include "string.h"

#define HISTORY_CAPACITY 16
#define HISTORY_ENTRY_SIZE 256

typedef struct {
    char entries[HISTORY_CAPACITY][HISTORY_ENTRY_SIZE];
    int head; // Next slot
    int count; // Entries count
    int cursor; // Nav pos
    char draft[HISTORY_ENTRY_SIZE]; // Current line
} history_t;

static history_t h;

int slot(int back) {
    return (h.head - back + HISTORY_CAPACITY) % HISTORY_CAPACITY;
}

void history_init(void) {
    h.head = 0;
    h.count = 0;
    h.cursor = 0;
    h.draft[0] = '\0';
}

void history_push(const char *cmd) {
    if (cmd == NULL || cmd[0] == '\0') return;
    strlcpy(h.entries[h.head], cmd, HISTORY_ENTRY_SIZE-1);
    h.count = h.count < HISTORY_CAPACITY ? h.count + 1 : HISTORY_CAPACITY;
    h.head = (h.head + 1) % HISTORY_CAPACITY;
    h.cursor = 0;
    h.draft[0] = '\0';
}

const char *history_prev(const char *current) {
    // cursor == 0 : on quitte l'édition, on met la ligne en cours de côté.
    if (h.cursor == 0 && current != NULL)
        strlcpy(h.draft, current, HISTORY_ENTRY_SIZE - 1);
    if (h.cursor < h.count)
        h.cursor++;
    return history_getcurrentcommand();
}

const char *history_next(void) {
    if (h.cursor > 0)
        h.cursor--;
    return history_getcurrentcommand();
}

const char *history_getcurrentcommand(void) {
    return h.cursor == 0 ? h.draft : h.entries[slot(h.cursor)];
}

int history_getcursor(void) {
    return h.cursor;
}

int history_getcount(void) {
    return h.count;
}

int history_gethead(void) {
    return h.head;
}
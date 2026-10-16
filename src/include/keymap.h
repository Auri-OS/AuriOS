#ifndef KEYMAP_H
#define KEYMAP_H

#include "integer.h"
#include "keyboard.h"

const char* keymap_parse(Key scancode);
void keymap_set_index(int i);

#endif
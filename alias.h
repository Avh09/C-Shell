#ifndef __ALIAS_H__
#define __ALIAS_H__
#include "headers.h"

typedef struct Alias {
    char *name;
    char *command;
    struct Alias *next;
} Alias;

unsigned int hash(const char *str);
void add_alias(const char *name, const char *command);
const char *get_alias(const char *name);
void load_aliases();
void print_aliases();


#endif
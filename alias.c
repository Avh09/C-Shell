#include "headers.h"


Alias *at[HASH_SIZE];


unsigned int hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = hash * 32;
        hash = hash + *str;
        str++;
    }
    return hash % HASH_SIZE;
}

void add_alias(const char *name, const char *command) {
    unsigned int index = hash(name);
    Alias *new_alias = malloc(sizeof(Alias));
    new_alias->name = strdup(name);
    new_alias->command = strdup(command);
    new_alias->next = at[index];
    at[index] = new_alias;
}

const char *get_alias(const char *name) {
    unsigned int index = hash(name);
    Alias *alias = at[index];
    while (alias) {
        if (strcmp(alias->name, name) == 0) {
            return alias->command;
        }
        alias = alias->next;
    }
    return NULL;
}

void print_aliases() {
    printf("Loaded Aliases:\n");
    for (int i = 0; i < HASH_SIZE; ++i) {
        Alias *alias = at[i];
        while (alias) {
            printf("alias %s='%s'\n", alias->name, alias->command);
            alias = alias->next;
        }
    }
}


void load_aliases() {
    char alias_file_path[MAX];
    strcpy(alias_file_path, shell_home_dir);
    strcat(alias_file_path, "/myshrc");
    FILE *file = fopen(alias_file_path, "r");
    if (!file) {
        perror("fopen");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';  
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }
        if (strstr(line, "alias") == line) {
            char *equals = strchr(line, '=');
            if (equals != NULL) {
                *equals = '\0'; 
                char *alias_name = line + 6;  
                char *command = equals + 1;

                alias_name = strtok(alias_name, " ");
                add_alias(alias_name, command);
            }
        }
    }
    fclose(file);
}
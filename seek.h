#ifndef __SEEK_H_
#define __SEEK_H_
#include "headers.h"

void seek_func(char **args, int ac, char *shell_home_dir);
void look(char *target_filename, char *target_path, int d, int e, int f);
void look_e(char *target_filename, char *target_path, int d, int f);
void print_relative_path(const char *absolute_path);

#endif
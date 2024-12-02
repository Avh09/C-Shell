#ifndef __REVEAL_H_
#define __REVEAL_H_
#include "headers.h"

void reveal(char **args, int ac, char *shell_home_dir);
void hop_prev_dir(char *cur_dir, char *prev_dir, int a, int l);
void hop_parent_dir(char *cur_dir, int a, int l);
void hop_cur_dir(char *cur_dir,int a,int l);
void hop_home_dir(char *shell_home_dir, int a, int l);
void hop_path(char *path, int a, int l);
int compare(const void *a, const void *b);
void print_files(char *dir, int a, int l);
void print_file_details(const char *filepath);

#endif
#ifndef __TOKEN_H__
#define __TOKEN_H__
#include "headers.h"

void execute_foreground_commands(char *command,char *shell_home_dir);
void execute_background_commands(char *command,char *shell_home_dir);
void process_input(char *input,char *shell_home_dir);
void process_input_execute(char *input,char *shell_home_dir);
void handle_sigchld(int sig);
void handle_sigint(int sig);
void handle_sigtstp(int sig);
void add_background_process(pid_t pid, char *command);
void remove_background_process(pid_t pid);
void update_background_process_status(pid_t pid, int new_status);

#endif
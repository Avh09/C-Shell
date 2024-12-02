#include "headers.h"

void directory(const char *shell_home_dir){
    char hostname[MAX];
    char username[MAX];
    char cwd[MAX];
    if(gethostname(hostname, sizeof(hostname)) == -1){
        perror("gethostname");
        return;
    }
    if(getlogin_r(username, sizeof(username)) == -1){
        perror("getlogin_r");
        return;
    }
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("getcwd");
        return;
    }
    printf(PINK "%s@%s:" RESET, username, hostname);
    if(strncmp(cwd, shell_home_dir, strlen(shell_home_dir)) == 0){
        printf(ORANGE "~%s" RESET, cwd + strlen(shell_home_dir));
    }
    else{
        printf(CYAN "%s" RESET, cwd);
    }
    if (strlen(last_long_running_command) > 0) {
        printf(ORANGE " %s" RESET, last_long_running_command);
        last_long_running_command[0] = '\0'; 
        printf(ORANGE "> " RESET);
    }
    else{
        printf(ORANGE "> " RESET);
    }
}
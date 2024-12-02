#include "headers.h"

extern BackgroundProcess *head;

void fg(char* pidstr){
    int pid = atoi(pidstr);
    remove_background_process(pid);
    if (kill(pid, 0) == -1) {
        perror("Process does not exist or cannot be accessed");
        return;
    }

    pid_t shell_pgid = getpgrp();
    
    signal(SIGTTIN, SIG_IGN);  
    signal(SIGTTOU, SIG_IGN);  

    tcsetpgrp(STDIN_FILENO, pid);

    kill(pid, SIGCONT);

    int status;
    waitpid(pid, &status, WUNTRACED);

    tcsetpgrp(0, shell_pgid);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    if (WIFSTOPPED(status)) {
        printf("Process %d stopped\n", pid);
    } else {
        printf("Process %d finished\n", pid);
    }
}

void bg(char *pid_str) {
    BackgroundProcess *current = head;
    int pid = atoi(pid_str); 
    while (current != NULL) {
        if (current->pid == pid) {
            if (current->status == 1) {
                kill(pid, SIGCONT);
                printf("hehehe\n");
                current->status = 0;
                return;
            } else {
                printf("Process with PID %d is already running.\n", pid);
                return;
            }
        }
        current = current->next;
    }
    printf("Error: Process with PID %d not found in background.\n", pid);
}
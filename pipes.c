#include "headers.h"

void whatever_just_pipe(char *command){
    process_input(command, shell_home_dir);
}

void piping(char *com, char *shell_home_dir) {
    // printf("com : %s\n", com);
    // com[strspn(com, "\n")] = '\0';
    char *com_list[MAX];
    int cnt = 0;

    char *token = strtok(com, "|");
    while (token != NULL) {
        com_list[cnt++] = token;
        token = strtok(NULL, "|");
    }

    // printf("Tokenized commands:\n");
    // for(int i = 0; i < cnt; i++){
    //     printf("Command %d: %s\n", i + 1, com_list[i]);
    // }

    int fd[2];
    int pfd = 0;

    for (int i = 0; i < cnt; i++) {
        pipe(fd);
        int pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            dup2(pfd, STDIN_FILENO);
            if (i < cnt - 1) {
                dup2(fd[1], STDOUT_FILENO);
            }
            close(fd[0]);
            whatever_just_pipe(com_list[i]);
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);
            close(fd[1]);
            pfd = fd[0];
        }
    }

    close(pfd);  
}

 



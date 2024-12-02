#include "headers.h"

char process_info[512] = "";
char last_long_running_command[512] = "";
char prev_dir[MAX] = "";             
char cur_dir[MAX];                   
char new_cur_dir[MAX];               
char full_path[MAX];
char store_prev_dir[MAX] = "";
char shell_home_dir[MAX];
char filepath[MAX];
char filename_seek[MAX];
char path_seek[MAX]="";
char path[MAX];
char relative_path[MAX];
char io_filepath[MAX];
char io_filename[MAX];
int is_redirected = 0;
char copy_command[MAX];
int sig_flag;
pid_t running_processes[MAX];
int process_count = 0;
int neo_flag = 0;

int main(){
    struct sigaction sa;
    sa.sa_handler = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }


    sa.sa_handler = &handle_sigint;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    sa.sa_handler = handle_sigtstp;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;  
    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }


    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigtstp);
    signal(SIGCHLD, handle_sigchld);

    char filename[] = "log.txt";
    int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(fd == -1){
        perror("open");
        return 1;
    }
    if (getcwd(shell_home_dir, sizeof(shell_home_dir)) == NULL) {
        perror("getcwd() error");
        return 1;
    }
    strcpy(filepath, shell_home_dir);
    strcat(filepath, "/");
    strcat(filepath, filename);

    load_aliases();
    // print_aliases();

    while(1){
        directory(shell_home_dir);
        char command[1024];

        if (fgets(command, MAX_INPUT, stdin) == NULL) {
            if (feof(stdin)) {
                for (int i = 0; i < process_count; ++i) {
                    kill(running_processes[i], SIGTERM);
                }
                while (waitpid(-1, NULL, WNOHANG) > 0);
                printf("\n");
                exit(0);
            } 
            else {
                perror("fgets");
                continue;
                // exit(1);
            }
        }
        command[strcspn(command, "\n")] = 0;
        if (strlen(command) == 0) {
            continue;
        }
        sig_flag = 0;
        process_input(command,shell_home_dir);
    }
}



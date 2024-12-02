#include "headers.h"

BackgroundProcess *bg_process_list = NULL;
BackgroundProcess *head = NULL;
int rc;
int fg_process = -1;
int process_stopped = 0;
char imdone[MAX];

void handle_sigtstp(int sig) {
    printf("***\n");
    if (fg_process > 0) {
        printf("\nProcess %d suspended and moved to the background.\n", fg_process);
        kill(fg_process, SIGTSTP); 
        process_stopped = 1;       
        add_background_process(fg_process, imdone);  
        update_background_process_status(fg_process, 1);
    }
}

void handle_sigint(int sig) {
    if (rc > 0) {
        kill(rc, SIGINT);
    } else {
        printf("\nNo foreground process to interrupt\n");
    }
}

void execute_foreground_commands(char *command, char *shell_home_dir) {
    // printf("IN FG\n");
    char copy_command[MAX];
    strcpy(copy_command,command);
    strcpy(imdone,command);
    char *args[MAX];
        int ac = 0;
        char *token = strtok(command, " ");
        while (token != NULL && ac < 100) {
            args[ac++] = token;

            token = strtok(NULL, " ");
        }
        args[ac] = NULL;
    if (strstr(copy_command, "|") != NULL) {
        char mybad[MAX];
        strcpy(mybad, copy_command);
        for (int i = 0; i < strlen(mybad) - 1; i++) {
            if (mybad[i] == '|' && mybad[i + 1] == '|') {
                printf(RED "ERROR : Invalid command! (Consecutive '|' characters)\n" RESET);
                return;
            }
        }
        for (int i = 0; i < ac - 1; i++) {
            if (strcmp(args[i], "|") == 0 && strcmp(args[i + 1], "|") == 0) {
                printf(RED "ERROR : Invalid command! (Consecutive '|' characters)\n" RESET);
                return;
            }
        }
        int len = strlen(copy_command);
        if(copy_command[len-1] == '|'){
            printf(RED "Error : No argument after |\n" RESET);
            return;
        }
        char *arr[MAX];
        int cnt = 0;
        char *tok = strtok(copy_command, "|");
        while (tok != NULL && cnt < 100) {
            arr[cnt++] = tok;
            tok = strtok(NULL, "|");
        }
        for (int i = 0; i < cnt - 1; i++) {
            if(arr[i] == NULL){
                printf(RED "ERROR : Invalid command! (Empty or whitespace argument)\n" RESET);
                return;
            }
            if (strcmp(arr[i], "|") == 0 && strcmp(arr[i + 1], "|") == 0) {
                printf(RED "ERROR : Invalid command! (Consecutive '|' characters)\n" RESET);
                return;
            }
        }
        if (cnt < 2 || arr[0] == NULL || arr[cnt - 1] == NULL ||
            strlen(arr[0]) == 0 || strlen(arr[cnt - 1]) == 0) {
            printf(RED "ERROR : Invalid command! (Empty argument)\n" RESET);
            return;
            }
            piping(mybad, shell_home_dir);
        }
        else if (strchr(copy_command, '>') != NULL || strchr(copy_command, '<') != NULL) {
            // printf("inside io\n");
            io(copy_command, shell_home_dir);
            return;
        }
        else if (strcmp(args[0], "hop") == 0) {
            hop(args + 1, ac - 1, shell_home_dir);
        } else if (strcmp(args[0], "reveal") == 0) {
            reveal(args + 1, ac - 1, shell_home_dir);
        } else if (strcmp(args[0], "log") == 0) {
            if (args[1] == NULL) {
                log_func();
            } else if (strcmp(args[1], "purge") == 0) {
                purge();
            } else if (strcmp(args[1], "execute") == 0) {
                int n = atoi(args[2]);
                log_execute(n);
            } else {
                printf("Unknown log subcommand: %s\n", args[1]);
            }
        } else if (strcmp(args[0], "proclore") == 0) {
            obtain_process_info(args + 1);
        } else if (strcmp(args[0], "seek") == 0) {
            seek_func(args + 1, ac - 1, shell_home_dir);
        } else if (strcmp(args[0], "activities") == 0) {
            print_activities();
        } else if (strcmp(args[0], "iMan") == 0) {
            // printf("*****\n");
            iman(args[1]);
        } else if (strcmp(args[0], "ping") == 0) {
            our_signal(args[1], args[2]);
        } else if(strcmp(args[0], "fg") == 0){
            fg(args[1]);
        } else if(strcmp(args[0], "bg") == 0){
            bg(args[1]);
        } else if(strcmp(args[0], "neonate") == 0){
            neo_flag = 1;
            neo(args[2]);
        }
        else if (strcmp(args[0], "exit") == 0) {
            exit(0);
        }
    else{
        // printf("EXEC\n");
        rc = fork();
        if (rc < 0) {
            perror("fork");
            exit(1);
        } else if (rc == 0) {
                setpgid(0,0);
                char *args[MAX];
                int ac = 0;
                char *token = strtok(copy_command, " ");
                while (token != NULL && ac < 100) {
                    args[ac++] = token;
                    token = strtok(NULL, " ");
                }
                args[ac] = NULL;
                if (execvp(args[0], args) == -1) {
                    printf(RED "ERROR : %s is not a valid command!\n" RESET, args[0]);
                    perror("execvp failed");
                    exit(EXIT_FAILURE);
                } 
        }        
        else {
            fg_process = rc;  
            time_t start_time = time(NULL);
            process_stopped = 0;
            int status;
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN); 
            tcsetpgrp(0, rc); 

            while (waitpid(rc, &status, WUNTRACED) > 0) {
                if (WIFSTOPPED(status)) {
                    printf("Process %d stopped.\n", rc);
                    process_stopped = 1;

                    add_background_process(fg_process, imdone);
                    update_background_process_status(fg_process, 1);

                    tcsetpgrp(0, getpgid(0));

                    signal(SIGTTIN, SIG_DFL);
                    signal(SIGTTOU, SIG_DFL);
                    break; 
                } 
                else if (WIFEXITED(status)) {
                    time_t end_time = time(NULL);
                    double time_taken = difftime(end_time, start_time);

                    int rounded_time = (int)(time_taken);
                    if (rounded_time > 2) {
                        char buffer[128];
                        snprintf(buffer, sizeof(buffer), "%s : %ds", command, rounded_time);

                        if (strlen(last_long_running_command) > 0) {
                            strncat(last_long_running_command, " | ", sizeof(last_long_running_command) - strlen(last_long_running_command) - 1);
                            strncat(last_long_running_command, buffer, sizeof(last_long_running_command) - strlen(last_long_running_command) - 1);
                        } else {
                            strncpy(last_long_running_command, buffer, sizeof(last_long_running_command) - 1);
                        }
                    }
                    break; 
                }
            }
            tcsetpgrp(0, getpgid(0));  
            signal(SIGTTIN, SIG_DFL);  
            signal(SIGTTOU, SIG_DFL); 
            fg_process = -1;
        }
    } 
}

void update_background_process_status(pid_t pid, int new_status) {
    BackgroundProcess *current = head;

    while (current != NULL) {
        if (current->pid == pid) {
            current->status = new_status;
            printf("Updated process with PID %d to status %d\n", pid, new_status);
            return;
        }
        current = current->next;
    }
    printf("Process with PID %d not found in the background list\n", pid);
}

void add_background_process(pid_t pid, char *command) {
    BackgroundProcess *current = head;
    while (current != NULL) {
        if (current->pid == pid) {
            return;
        }
        current = current->next;
    }
    BackgroundProcess *new_process = malloc(sizeof(BackgroundProcess));
    if (new_process == NULL) {
        perror("malloc failed");
        return;
    }
    
    new_process->pid = pid;
    strncpy(new_process->command, command, sizeof(new_process->command) - 1);
    new_process->command[sizeof(new_process->command) - 1] = '\0';
    new_process->status = 0;
    new_process->next = head;
    head = new_process;
}

void remove_background_process(pid_t pid) {
    // printf("****\n");
    BackgroundProcess *current = head;
    BackgroundProcess *prev = NULL;
    while (current != NULL) {
        if (current->pid == pid) {
            if (prev == NULL) {
                // printf("##########%d\n",head->pid);
                // printf("##########%d\n",current->pid);
                head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("PID %d not found in the list\n", pid);
}

void handle_sigchld(int sig) {
    int status;
    pid_t pid;
    // printf("IN SIGCHLD\n");
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
        BackgroundProcess *current = head;
        while (current != NULL) {
            if (current->pid == pid) {
                if (WIFEXITED(status)) {
                    printf("%s exited normally (PID %d)\n", current->command, pid);
                    remove_background_process(pid);  
                } 
                else if (WIFSIGNALED(status)) {
                    // printf("####\n");
                    int signal_number = WTERMSIG(status);
                    // printf("%s was terminated by signal %d (PID %d)\n", current->command, signal_number, pid);
                    remove_background_process(pid); 
                }  
                else if (WIFSTOPPED(status)) {
                    // printf("****\n");
                    current->status = 1;
                    int signal_number = WSTOPSIG(status);
                }
               else if (WIFCONTINUED(status)) {
                    printf("%s resumed (PID %d)\n", current->command, pid);
                    current->status = 0; 
                }
                break; 
            }
            current = current->next;
        }
    }
}

void execute_background_commands(char *command, char *shell_home_dir) {
    // printf("Background: %s\n", command);
    // printf("IN BG\n");
    char copy_command[MAX];
    strcpy(copy_command,command);
    int brc = fork();
    char *args[512];
    int ac = 0;
    char *token = strtok(command, " ");
    while (token != NULL && ac < 100) {
        args[ac++] = token;
        token = strtok(NULL, " ");
    }
        args[ac] = NULL;
    if (brc < 0) {
        perror("fork");
        exit(1);
    } else if (brc == 0) {
        setpgid(0,0);
        if (strstr(copy_command, "|") != NULL) {
            char mybad[MAX];
            strcpy(mybad, copy_command);
            for (int i = 0; i < strlen(mybad) - 1; i++) {
                if (mybad[i] == '|' && mybad[i + 1] == '|') {
                    printf(RED "ERROR : Invalid command! (Consecutive '|' characters)\n" RESET);
                    return;
                }
            }
            char *arr[MAX];
            int cnt = 0;
            char *tok = strtok(copy_command, "|");
            while (tok != NULL && cnt < 100) {
                arr[cnt++] = tok;
                tok = strtok(NULL, "|");
            }
            for (int i = 0; i < cnt - 1; i++) {
                if(arr[i] == NULL){
                    printf(RED "ERROR : Invalid command! (Empty or whitespace argument)\n" RESET);
                    return;
                }
                if (strcmp(arr[i], "|") == 0 && strcmp(arr[i + 1], "|") == 0) {
                    printf(RED "ERROR : Invalid command! (Consecutive '|' characters)\n" RESET);
                    return;
                }
            }
            if (cnt < 2 || arr[0] == NULL || arr[cnt - 1] == NULL ||
                strlen(arr[0]) == 0 || strlen(arr[cnt - 1]) == 0) {
                printf(RED "ERROR : Invalid command! (Empty or whitespace argument)\n" RESET);
                return;
            }
            piping(mybad, shell_home_dir);
        }   

        else if (strchr(copy_command, '>') != NULL || strchr(copy_command, '<') != NULL) {
            // printf("inside io\n");
            io(copy_command, shell_home_dir);
            return;
        }
        else{
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        } 
    } 
    else {
        // add_process(rc, args[0]);
        add_background_process(brc, args[0]);
        printf("(%d)\n", brc);
        running_processes[process_count++] = brc;
    }
}

void process_input(char *input, char *shell_home_dir) {
    // print_activities();
    // printf("PROCESS INPUT\n");
    // printf("lalala\n");
    const char *expanded_command = get_alias(input);
    char send_input[MAX];
    // strcpy(send_input, expanded_command);
    if (expanded_command) {
        strcpy(send_input, expanded_command);
        execute_foreground_commands(send_input, shell_home_dir);
    } else {
        char *segment;
        char *saveptr;
        if (strstr(input, "log") == NULL) {
                log_thing(input);
        }
        segment = strtok_r(input, ";", &saveptr);
        while (segment != NULL) {
            char *token = strtok(segment, " \t");
            if (token == NULL) {
                segment = strtok_r(NULL, ";", &saveptr);
                continue;
            }
            char command[4096] = {0};
            while (token != NULL) {
                strcat(command, token);
                strcat(command, " "); 
                token = strtok(NULL, " \t");
            }
            size_t len = strlen(command);
            if (len > 0 && command[len - 1] == ' ') {
                command[len - 1] = '\0';
            }
            char *command_trimmed = command;
            char *ampersand_pos;
            while ((ampersand_pos = strchr(command_trimmed, '&')) != NULL) {
                *ampersand_pos = '\0'; 
                char *before_ampersand = command_trimmed;
                char *before_end = before_ampersand + strlen(before_ampersand) - 1;
                while (before_end > before_ampersand && (*before_end == ' ' || *before_end == '\t')) before_end--;
                *(before_end + 1) = '\0';

                execute_background_commands(before_ampersand, shell_home_dir);

                command_trimmed = ampersand_pos + 1;
                while (*command_trimmed == ' ' || *command_trimmed == '\t') command_trimmed++;
            }
            if (strlen(command_trimmed) > 0) {
                char *end = command_trimmed + strlen(command_trimmed) - 1;
                while (end > command_trimmed && (*end == ' ' || *end == '\t')) end--;
                *(end + 1) = '\0';

                execute_foreground_commands(command_trimmed, shell_home_dir);
            }
            segment = strtok_r(NULL, ";", &saveptr);
        }
    }    
}

void process_input_execute(char *input,char *shell_home_dir) {
    const char *expanded_command = get_alias(input);
    char send_input[MAX];
    if (expanded_command) {
        strcpy(send_input, expanded_command);
        execute_foreground_commands(send_input, shell_home_dir);
    }
    else{
        char *segment;
        char *saveptr;

        segment = strtok_r(input, ";", &saveptr);
        while (segment != NULL) {
            char *token = strtok(segment, " \t");
            if (token == NULL) {
                segment = strtok_r(NULL, ";", &saveptr);
                continue;
            }

            char command[4096] = {0};
            while (token != NULL) {
                strcat(command, token);
                strcat(command, " "); 
                token = strtok(NULL, " \t");
            }
            size_t len = strlen(command);
            if (len > 0 && command[len - 1] == ' ') {
                command[len - 1] = '\0';
            }

            char *command_trimmed = command;
            char *ampersand_pos;
            while ((ampersand_pos = strchr(command_trimmed, '&')) != NULL) {
                *ampersand_pos = '\0'; 
                char *before_ampersand = command_trimmed;
                char *before_end = before_ampersand + strlen(before_ampersand) - 1;
                while (before_end > before_ampersand && (*before_end == ' ' || *before_end == '\t')) before_end--;
                *(before_end + 1) = '\0';

                execute_background_commands(before_ampersand, shell_home_dir);

                command_trimmed = ampersand_pos + 1;
                while (*command_trimmed == ' ' || *command_trimmed == '\t') command_trimmed++;
            }

            if (strlen(command_trimmed) > 0) {
                char *end = command_trimmed + strlen(command_trimmed) - 1;
                while (end > command_trimmed && (*end == ' ' || *end == '\t')) end--;
                *(end + 1) = '\0';

                execute_foreground_commands(command_trimmed, shell_home_dir);
            }

            segment = strtok_r(NULL, ";", &saveptr);
        }
    }
}

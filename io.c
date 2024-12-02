#include "headers.h"

void io(char *command, char *shell_home_dir){

    // printf("command%c",command[0]);
    if(command[0] == '>'|| command[0] == '<' ){
        printf("Nothing to send/receive in io\n");
        return;
    }
    is_redirected = 1;
    char curdir[MAX];
    if (getcwd(curdir, sizeof(curdir)) == NULL) {
        perror("getcwd() error");
        return;
    }
    int us = 0;
    char before_redirect[MAX];
    char copy_command[MAX];
    char iofilename[MAX];

    char *output_redirect = strstr(command, ">");
    // char *output_redirect_append = strstr(command, ">>");
    char *input_redirect = strstr(command, "<");

    if (output_redirect != NULL && input_redirect != NULL) {
        // Check if '>' comes before '<'
        if (output_redirect < input_redirect) {
            printf(RED "Error: Output redirection '>' comes before input redirection '<'\n" RESET);
            return;
        } 
    } 
    // strcpy(before_redirect, token);
    if (strstr(command, "<") != NULL) {
        us = 3; 
    }
    else if (strstr(command, ">") != NULL) {
        if (strstr(command, ">>") != NULL) {
            us = 2;
        } else {
            us = 1; 
        }
    } 
    
    strcpy(copy_command, command);

    if(us == 1 || us == 2){
        char *input_redirect = strstr(command, "<");
        if(input_redirect != NULL){
            char *ar[512];
            int c = 0;
            char *token = strtok(command, "<");
            if (token != NULL) {
                strcpy(before_redirect, token);
                token = strtok(NULL, "<"); 
            }
            while (token != NULL && c < 100) {
                ar[c++] = token;
                token = strtok(NULL, "<");
            }
            ar[c] = NULL;

            strcpy(command, copy_command);
            char *args[512];
            int ac = 0;
            token = strtok(command, " "); 
            while (token != NULL && ac < 100) {
                args[ac++] = token;
                token = strtok(NULL, " ");
            }
            args[ac] = NULL;
            char iofilename[MAX];
            strcpy(iofilename, args[ac-1]);
            strcpy(io_filename, iofilename);
            strcpy(io_filepath, curdir);
            strcat(io_filepath, "/");
            strcat(io_filepath, io_filename);

            // reads input from the file
            int fd2 = open(io_filepath, O_RDONLY);
            if(fd2 < 0){
                printf(RED "Error: File not found\n" RESET);
                exit(1);
            }
            int saving_fd_of_stdin2 = dup(STDIN_FILENO);
            if(saving_fd_of_stdin2 == -1){
                perror("dup");
                close(fd2);
                exit(1);
            }
            int rv2 = dup2(fd2, STDIN_FILENO);
            if(rv2 == -1){
                perror("dup2");
                close(fd2);
                exit(1);
            }
            else{
                args[ac-1] = NULL;
                args[ac-2] = NULL;
                // printf("before_redirect : %s\n",before_redirect);
                process_input(before_redirect, shell_home_dir);
                rv2 = dup2(saving_fd_of_stdin2, STDIN_FILENO);
                if(rv2 == -1){
                    perror("dup2");
                    close(fd2);
                    exit(1);
                }
                close(fd2);
                close(saving_fd_of_stdin2);

            }
        }
        else{
            // printf("^^^^^\n");
            char *ar[512];
            int c = 0;
            char *token = strtok(command, ">>");
            if (token != NULL) {
                strcpy(before_redirect, token);
                token = strtok(NULL, ">>"); 
            }
            while (token != NULL && c < 100) {
                ar[c++] = token;
                token = strtok(NULL, ">>");
            }
            ar[c] = NULL;

            strcpy(command, copy_command);
            char *args[512];
            int ac = 0;
            token = strtok(command, " "); 
            while (token != NULL && ac < 100) {
                args[ac++] = token;
                token = strtok(NULL, " ");
            }
            args[ac] = NULL;

            strcpy(iofilename, args[ac-1]);
            strcpy(io_filename, iofilename);
            strcpy(io_filepath, curdir);
            strcat(io_filepath, "/");
            strcat(io_filepath, io_filename);
            // printf("io_filepath : %s us : %d\n",io_filepath, us);
            int fd;
            if(us == 1){
                fd = open(io_filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                // printf("fd : %d\n",fd);
            }
            else{
                fd = open(io_filepath, O_WRONLY | O_CREAT | O_APPEND, 0644); 
            }

            if(fd < 0){
                perror("open");
                exit(1);
            }
            int saving_fd_of_stdout = dup(STDOUT_FILENO);
            if(saving_fd_of_stdout == -1){
                perror("dup");
                close(fd);
                exit(1);
            }
            int rv = dup2(fd, STDOUT_FILENO);
            if(rv == -1){
                perror("dup2");
                close(fd);
                exit(1);
            }
            else{
                args[ac-1] = NULL;
                args[ac-2] = NULL;
                // printf("before_redirect : %s\n",before_redirect);
                process_input(before_redirect, shell_home_dir);
                rv = dup2(saving_fd_of_stdout, STDOUT_FILENO);
                if(rv == -1){
                    perror("dup2");
                    close(fd);
                    exit(1);
                }
                close(fd);
                close(saving_fd_of_stdout);
            }
        }  
    }
    else if(us == 3){
        char *output_redirect = strstr(command, ">>");
        int redirect_type = 0;  // 1 for '>>', 2 for '>'

        if (output_redirect == NULL) {
            output_redirect = strstr(command, ">");
            redirect_type = 2;  // Indicate '>'
        } else {
            redirect_type = 1;  // Indicate '>>'
        }

        if (output_redirect != NULL) {
            // Extract the segment before '>' or '>>'
            char before_output_redirect[512];
            size_t length = output_redirect - command;
            strncpy(before_output_redirect, command, length);
            before_output_redirect[length] = '\0';  // Null-terminate the string

            // Extract the segment after '>' or '>>' (filename or further command)
            char *after_output_redirect = output_redirect + (redirect_type == 1 ? 2 : 1);  // Skip '>>' or '>'

            // Trim leading spaces in the segment after the redirection symbol
            while (*after_output_redirect == ' ') {
                after_output_redirect++;
            }

            // Print the parts
            // printf("Segment before '>' or '>>': %s\n", before_output_redirect);
            // printf("Segment after '>' or '>>' (filename): %s\n", after_output_redirect);
            strcpy(iofilename, after_output_redirect);
            strcpy(io_filename, iofilename);
            strcpy(io_filepath, curdir);
            strcat(io_filepath, "/");
            strcat(io_filepath, io_filename);
            // printf("io_filepath : %s\n",io_filepath);
            int fd;
            if(redirect_type == 1){
                fd = open(io_filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            else{
                fd = open(io_filepath, O_WRONLY | O_CREAT | O_APPEND, 0644); 
            }
            if(fd < 0){
            perror("open");
            exit(1);
            }
            int saving_fd_of_stdout = dup(STDOUT_FILENO);
            if(saving_fd_of_stdout == -1){
                perror("dup");
                close(fd);
                exit(1);
            }
            int rv = dup2(fd, STDOUT_FILENO);
            if(rv == -1){
                perror("dup2");
                close(fd);
                exit(1);
            }
            process_input(before_output_redirect, shell_home_dir);
            rv = dup2(saving_fd_of_stdout, STDOUT_FILENO);
            if(rv == -1){
                perror("dup2");
                close(fd);
                exit(1);
            }
            close(fd);
            close(saving_fd_of_stdout);
        }
        else {
            // No '>' or '>>' found, print the entire command
            strcpy(command, copy_command);
            char *ar[512];
            int c = 0;
            char *token = strtok(command, "<");
            if (token != NULL) {
                strcpy(before_redirect, token);
                token = strtok(NULL, "<"); 
            }
            while (token != NULL && c < 100) {
                ar[c++] = token;
                token = strtok(NULL, "<");
            }
            ar[c] = NULL;

            strcpy(command, copy_command);
            char *args[512];
            int ac = 0;
            token = strtok(command, " "); 
            while (token != NULL && ac < 100) {
                args[ac++] = token;
                token = strtok(NULL, " ");
            }
            args[ac] = NULL;
            char iofilename[MAX];
            strcpy(iofilename, args[ac-1]);
            strcpy(io_filename, iofilename);
            strcpy(io_filepath, curdir);
            strcat(io_filepath, "/");
            strcat(io_filepath, io_filename);

            // reads input from the file
            int fd2 = open(io_filepath, O_RDONLY);
            if(fd2 < 0){
                printf(RED "Error: File not found\n" RESET);
                exit(1);
            }
            int saving_fd_of_stdin2 = dup(STDIN_FILENO);
            if(saving_fd_of_stdin2 == -1){
                perror("dup");
                close(fd2);
                exit(1);
            }
            int rv2 = dup2(fd2, STDIN_FILENO);
            if(rv2 == -1){
                perror("dup2");
                close(fd2);
                exit(1);
            }
            else{
                args[ac-1] = NULL;
                args[ac-2] = NULL;
                // printf("before_redirect : %s\n",before_redirect);
                process_input(before_redirect, shell_home_dir);
                rv2 = dup2(saving_fd_of_stdin2, STDIN_FILENO);
                if(rv2 == -1){
                    perror("dup2");
                    close(fd2);
                    exit(1);
                }
                close(fd2);
                close(saving_fd_of_stdin2);
            }
            // printf("No output redirection found, command: %s\n", command);
        }
    }
}

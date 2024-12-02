#include "headers.h"

void extract_value(const char *line, const char *prefix, char *output) {
    const char *start = strstr(line, prefix);
    if (start) {
        start += strlen(prefix); 
        while (isspace((unsigned char)*start)) start++;
        if (strncmp(prefix, "State:", 6) == 0) {
            if (*start != '\0') {
                *output = *start; 
                *(output + 1) = '\0';  
            }
        } else {
            strcpy(output, start); 
        }
    }
}

void obtain_process_info(char **pid) {
    char buffer[4096];
    char path[1024];
    int fd;

    pid_t pid_num;
    if (pid[0] == NULL) {
        pid_num = getpid();
    } else {
        pid_num = atoi(pid[0]);
    }
    snprintf(path, sizeof(path), "/proc/%d/status", pid_num);

    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("read");
        close(fd);
        return;
    }
    buffer[bytes_read] = '\0';  
    close(fd);

    char pid_value[32] = {0};
    char state_value[2] = {0};  // Updated to hold only 1 character + null terminator
    char tgid_value[32] = {0};
    char vmsize_value[32] = {0};

    char *line = strtok(buffer, "\n");
    while (line != NULL) {
        if (strncmp(line, "Pid:", 4) == 0) {
            extract_value(line, "Pid:", pid_value);
        } else if (strncmp(line, "State:", 6) == 0) {
            extract_value(line, "State:", state_value);
        } else if (strncmp(line, "Tgid:", 5) == 0) {
            extract_value(line, "Tgid:", tgid_value);
        } else if (strncmp(line, "VmSize:", 7) == 0) {
            extract_value(line, "VmSize:", vmsize_value);
        }
        line = strtok(NULL, "\n");
    }

    pid_t pgid = getpgid(pid_num);
    pid_t fg_pgid = tcgetpgrp(STDIN_FILENO);

    if (pgid == fg_pgid) {
        strcat(state_value, "+");
    }

    if (pid_value[0] != '\0') printf("Pid: %s\n", pid_value);
    if (state_value[0] != '\0') printf("Process status: %s\n", state_value);
    if (tgid_value[0] != '\0') printf("Process Group: %s\n", tgid_value);
    if (vmsize_value[0] != '\0') printf("Virtual Memory: %s\n", vmsize_value);
    
    snprintf(path, sizeof(path), "/proc/%d/exe", pid_num);
    char exe_path[1024];
    ssize_t len = readlink(path, exe_path, sizeof(exe_path) - 1);
    if (len != -1) {
        exe_path[len] = '\0';
        printf("Executable Path: %s\n", exe_path);
    } else {
        perror("readlink");
    }
}



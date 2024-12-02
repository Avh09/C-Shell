#include "headers.h"

char lines[MAX_LINES][MAX_LINE_LENGTH] = {0};
char lines_execute[MAX_LINES][MAX_LINE_LENGTH] = {0};
int line_count = 0;
int line_count_execute = 0;

void shift_lines(char lines[MAX_LINES][MAX_LINE_LENGTH], int line_count) {
    for (int i = 1; i < line_count; i++) {
        strcpy(lines[i - 1], lines[i]);
    }
}

void write_lines_to_file(char *filename, char lines[MAX_LINES][MAX_LINE_LENGTH], int line_count) {
    int fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd == -1) {
        perror("open() error");
        return;
    }

    for (int i = 0; i < line_count; i++) {
        if (write(fd, lines[i], strlen(lines[i])) == -1) {
            perror("write() error");
            close(fd);
            return;
        }
        if (write(fd, "\n", 1) == -1) {
            perror("write() error");
            close(fd);
            return;
        }
    }

    close(fd);
}

void read_lines_from_file(char *filename, char lines[MAX_LINES][MAX_LINE_LENGTH], int *line_count) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open() error");
        return;
    }

    char buffer[MAX_LINE_LENGTH];
    ssize_t bytes_read;
    size_t index = 0;
    size_t char_index = 0;


    for (int i = 0; i < MAX_LINES; i++) {
        lines[i][0] = '\0';
    }

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';

        for (ssize_t i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                if (index < MAX_LINES) {
                    lines[index][char_index] = '\0';
                    index++;
                    char_index = 0;
                }
            } else {
                if (char_index < MAX_LINE_LENGTH - 1) {
                    lines[index][char_index++] = buffer[i];
                }
            }
        }
    }

    if (char_index > 0 && index < MAX_LINES) {
        lines[index][char_index] = '\0';
        index++;
    }

    *line_count = index;

    close(fd);
}

void write_to_log_file(char *command) {

    read_lines_from_file(filepath, lines, &line_count);

    if (line_count >= MAX_LINES) {
        shift_lines(lines, line_count);
        strcpy(lines[MAX_LINES - 1], command);
        line_count = MAX_LINES;
    } else {
        if (line_count < MAX_LINES) {
            strcpy(lines[line_count], command);
            line_count++;
        }
    }
    write_lines_to_file(filepath, lines, line_count);
}

void log_func() {
    // printf("lalalal\n");
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("open() error");
        return;
    }
    char buffer[4096];
    int n;
    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        if (write(STDOUT_FILENO, buffer, n) == -1) {
            perror("write() error");
            return;
        }
    }
    if (n == -1) {
        perror("read() error");
        return;
    }
    close(fd);
}

void purge() {
    // printf("Purging log file\n");
    int fd = open(filepath, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        perror("open() error");
        return;
    }
    close(fd);
}

void log_thing(char *command) {
    if (strstr(command, "log") != NULL) {
        return;
    }

    read_lines_from_file(filepath, lines, &line_count);

    // printf("Line count: %d\n", line_count);
    // printf("Command to log: '%s'\n", command);
    if (line_count > 0) {
        // printf("Last logged command: '%s'\n", lines[line_count - 1]);
    }

    if (line_count > 0 && strcmp(lines[line_count - 1], command) == 0) {
        // printf("Skipping duplicate command.\n");
        return; 
    }

    if (line_count >= MAX_LINES) {
        // printf("Log is full. Shifting lines.\n");
        shift_lines(lines, line_count);
        strcpy(lines[MAX_LINES - 1], command);
    } else {
        // printf("Logging new command.\n");
        strcpy(lines[line_count], command);
        line_count++;
    }
    write_lines_to_file(filepath, lines, line_count);

    // printf("Command '%s' logged successfully.\n", command);
}

void log_execute(int val){
    read_lines_from_file(filepath, lines_execute, &line_count_execute);
    // for (int i = 0; i < line_count_execute; i++) {
    //     printf("%s\n", lines_execute[i]);
    // }
    int count = 0;
    for(int i=line_count_execute-1; i>=0; i--){
        count++;
        if(count == val){
            process_input_execute(lines_execute[i],shell_home_dir);
        }
    }
}
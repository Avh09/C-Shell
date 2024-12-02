#include "headers.h"

struct termios orig_termios;

void term() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);  
}

void raw() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(term);
    raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);  
}

pid_t hehe() {
    DIR *dir;
    struct dirent *entry;
    pid_t max_pid = 0;

    if ((dir = opendir("/proc")) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            pid_t pid = atoi(entry->d_name);
            if (pid > 0 && pid > max_pid) {
                max_pid = pid;  
            }
        }
        closedir(dir);
    } else {
        perror("opendir failed");
        exit(EXIT_FAILURE);
    }
    return max_pid;
}

void neo(char *time) {
    int time_arg = atoi(time);
    raw(); 

    while (1) {
        pid_t latest_pid = hehe();
        printf("%d\n", latest_pid);
        fflush(stdout);
        for (int i = 0; i < time_arg * 10; i++) {
            usleep(100000);  
            char ch;
            if (read(STDIN_FILENO, &ch, 1) == 1) {  
                if (ch == 'x') {
                    goto cleanup;
                }
            }
        }
    }

cleanup:
    term();  
}


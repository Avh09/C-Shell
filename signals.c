#include "headers.h"

void our_signal(char *pid, char *signal) {
    sig_flag = 1;  
    int pid_int = atoi(pid);
    int signal_int = atoi(signal) % 32;
    if (kill(pid_int, signal_int) == 0) {
        printf("Sent signal %d to process with pid %d.\n", signal_int, pid_int);
    } else {
        perror("Error sending signal");
    }
}
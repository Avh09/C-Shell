#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "headers.h"


typedef struct BackgroundProcess {
    pid_t pid;
    char command[MAX]; 
    int status;
    struct BackgroundProcess *next;
} BackgroundProcess;

#endif

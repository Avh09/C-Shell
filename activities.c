#include "headers.h"

extern BackgroundProcess *head;

void swap(BackgroundProcess *a, BackgroundProcess *b) {
    pid_t temp_pid = a->pid;
    char temp_command[MAX];
    strcpy(temp_command, a->command);
    int temp_status = a->status;

    a->pid = b->pid;
    strcpy(a->command, b->command);
    a->status = b->status;

    b->pid = temp_pid;
    strcpy(b->command, temp_command);
    b->status = temp_status;
}

void sort_by_pid() {
    if (head == NULL) {
        return;
    }
    int swapped;
    BackgroundProcess *ptr;
    BackgroundProcess *last = NULL;
    do {
        swapped = 0;
        ptr = head;
        while (ptr->next != last) {
            if (ptr->pid > ptr->next->pid) {
                swap(ptr, ptr->next);
                swapped = 1;
            }
            ptr = ptr->next;
        }
        last = ptr;
    } while (swapped);
}

void print_activities() {
    sort_by_pid();
    BackgroundProcess *current = head;
    if (current == NULL) {
        printf("No background processes.\n");
        return;
    }
    while (current != NULL) {
        printf("%d : %s - ", current->pid, current->command);
        if(current->status == 0) {
            printf("Running\n");
        } else {
            printf("Stopped\n");
        }
        current = current->next;
    }
}

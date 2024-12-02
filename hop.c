#include "headers.h"

void hop(char **args, int ac, const char *shell_home_dir) {
    if (getcwd(cur_dir, sizeof(cur_dir)) == NULL) {
        printf(RED "Error getting current directory" RESET "\n");
        return;
    }

    if (ac == 0) {
        if (chdir(shell_home_dir) == 0) {
            printf(CYAN "%s\n" RESET, shell_home_dir);
            strcpy(prev_dir, cur_dir);
        } else {
            printf(RED "Error changing to home directory" RESET "\n");
        }
        return;
    }

    for (int i = 0; i < ac; i++) {
        char *path = args[i];
        int action_code = 0; 

        if (strcmp(path, ".") == 0) {
            action_code = 1;
        } else if (strcmp(path, "~") == 0) {
            action_code = 2;
        } else if (strcmp(path, "-") == 0) {
            action_code = 3;
        } else if (strcmp(path, "..") == 0) {
            action_code = 4;
        } else if (strncmp(path, "~/", 2) == 0) {
            action_code = 5;
        } else {
            action_code = 6;
        }

        switch (action_code) {
            case 1: //same
                if(!is_redirected){
                    printf(CYAN "%s\n" RESET, cur_dir);
                }
                else{
                    printf("%s\n", cur_dir);
                }
                strcpy(store_prev_dir, cur_dir);
                break;

            case 2: //home
                if (chdir(shell_home_dir) == 0) {
                    strcpy(prev_dir, cur_dir);
                    strcpy(store_prev_dir, prev_dir);
                    if(!is_redirected){
                        printf(CYAN "%s\n" RESET, shell_home_dir);
                    }
                    else{
                        printf("%s\n", shell_home_dir);
                    }
                    // printf(CYAN "%s\n" RESET, shell_home_dir);
                    strcpy(cur_dir, shell_home_dir);
                } else {
                    if(!is_redirected){
                        printf(RED "Error changing to home directory" RESET "\n");
                    }
                    else{
                        printf("Error changing to home directory\n");
                    }
                    // printf(RED "Error changing to home directory" RESET "\n");
                }
                break;

            case 3: //prev
                if (strlen(prev_dir) == 0) {
                    printf(RED "Error: No previous directory stored." RESET "\n");
                } else {
                    if (chdir(prev_dir) == 0) {
                        if(!is_redirected){
                            printf(CYAN "%s\n" RESET, prev_dir);
                        }
                        else{
                            printf("%s\n", prev_dir);
                        }
                        // printf(CYAN "%s\n" RESET, prev_dir);
                        strcpy(store_prev_dir, prev_dir);
                        strcpy(new_cur_dir, prev_dir);
                        strcpy(prev_dir, cur_dir);
                        strcpy(cur_dir, new_cur_dir);
                    } else {
                        if(!is_redirected){
                            printf(RED "Error changing to previous directory" RESET "\n");
                        }
                        else{
                            printf("Error changing to previous directory\n");
                        }
                        // printf(RED "Error changing to previous directory" RESET "\n");
                    }
                }
                break;

            case 4: //parent
                if (chdir("..") == 0) {
                    strcpy(prev_dir, cur_dir);
                    strcpy(store_prev_dir, prev_dir);
                    if (getcwd(cur_dir, sizeof(cur_dir)) != NULL) {
                        if(!is_redirected){
                            printf(CYAN "%s\n" RESET, cur_dir);
                        }
                        else{
                            printf("%s\n", cur_dir);
                        }
                        // printf(CYAN "%s\n" RESET, cur_dir);
                    } else {
                        if(!is_redirected){
                            printf(RED "Error getting current directory after changing to parent" RESET "\n");
                        }
                        else{
                            printf("Error getting current directory after changing to parent\n");
                        }
                        // printf(RED "Error getting current directory after changing to parent" RESET "\n");
                    }
                } else {
                    if(!is_redirected){
                        printf(RED "Error changing to parent directory" RESET "\n");
                    }
                    else{
                        printf("Error changing to parent directory\n");
                    }
                    // printf(RED "Error changing to parent directory" RESET "\n");
                }
                break;

            case 5: //relative
                snprintf(full_path, sizeof(full_path), "%s%s", shell_home_dir, path + 1);
                if (chdir(full_path) == 0) {
                    strcpy(prev_dir, cur_dir);
                    strcpy(store_prev_dir, prev_dir);
                    if (getcwd(cur_dir, sizeof(cur_dir)) != NULL) {
                        if(!is_redirected){
                            printf(CYAN "%s\n" RESET, cur_dir);
                        }
                        else{
                            printf("%s\n", cur_dir);
                        }
                        // printf(CYAN "%s\n" RESET, cur_dir);
                    } else {
                        if(!is_redirected){
                            printf(RED "Error getting current directory after changing to specified path" RESET "\n");
                        }
                        else{
                            printf("Error getting current directory after changing to specified path\n");
                        }
                        // printf(RED "Error getting current directory after changing to specified path" RESET "\n");
                    }
                } else {
                    if(!is_redirected){
                        printf(RED "Error changing to specified path" RESET "\n");
                    }
                    else{
                        printf("Error changing to specified path\n");
                    }
                    // printf(RED "Error changing to specified path" RESET "\n");
                }
                break;

            case 6: //specific
                if (chdir(path) == 0) {
                    strcpy(prev_dir, cur_dir);
                    strcpy(store_prev_dir, prev_dir);
                    if (getcwd(cur_dir, sizeof(cur_dir)) != NULL) {
                        if(!is_redirected){
                            printf(CYAN "%s\n" RESET, cur_dir);
                        }
                        else{
                            printf("%s\n", cur_dir);
                        }
                        // printf(CYAN "%s\n" RESET, cur_dir);
                    } else {
                        if(!is_redirected){
                            printf(RED "Error getting current directory after changing to specified path" RESET "\n");
                        }
                        else{
                            printf("Error getting current directory after changing to specified path\n");
                        }
                        // printf(RED "Error getting current directory after changing to specified path" RESET "\n");
                    }
                } else {
                    if(!is_redirected){
                        printf(RED "Error changing to specified path" RESET "\n");
                    }
                    else{
                        printf("Error changing to specified path\n");
                    }
                    // printf(RED "Error changing to specified path" RESET "\n");
                }
                break;

            default:
                if(!is_redirected){
                    printf(RED "Error: Unrecognized action." RESET"\n");
                }
                else{
                    printf("Error: Unrecognized action.\n");
                }
                // printf(RED "Error: Unrecognized action." RESET"\n");
                break;
        }
    }
}

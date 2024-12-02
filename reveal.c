#include "headers.h"

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void print_file_details(const char *filepath) {
    struct stat fileStat;
    if (stat(filepath, &fileStat) < 0) {
        perror("stat");
        return;
    }

    printf("%c", (S_ISDIR(fileStat.st_mode)) ? 'd' : '-');
    printf("%c", (fileStat.st_mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (fileStat.st_mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (fileStat.st_mode & S_IXUSR) ? 'x' : '-');
    printf("%c", (fileStat.st_mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (fileStat.st_mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (fileStat.st_mode & S_IXGRP) ? 'x' : '-');
    printf("%c", (fileStat.st_mode & S_IROTH) ? 'r' : '-');
    printf("%c", (fileStat.st_mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (fileStat.st_mode & S_IXOTH) ? 'x' : '-');
    printf(" %ld", fileStat.st_nlink);
    printf(" %s", getpwuid(fileStat.st_uid)->pw_name);
    printf(" %s", getgrgid(fileStat.st_gid)->gr_name);
    printf(" %5ld", fileStat.st_size);

    char time_buf[80];
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", localtime(&fileStat.st_mtime));
    printf(" %s", time_buf);

    printf(" %s\n", filepath);
}

void print_files(char *direc, int a, int l){
    struct dirent *entry;
    // printf("Path passed to opendir: %s\n", direc);

    DIR *dir = opendir(direc);

    if (dir == NULL) {
        printf(RED "Error: can't open directory\n" RESET);
        return;
    }

    char **filenames = NULL;
    int file_count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (!a && entry->d_name[0] == '.')
            continue;

        filenames = realloc(filenames, sizeof(char *) * (file_count + 1));
        filenames[file_count] = strdup(entry->d_name);
        file_count++;
    }
    
    qsort(filenames, file_count, sizeof(char *), compare);

    int count = 0;  
    int max_files_per_line = 1;  

    for (int i = 0; i < file_count; i++) {
        if (l) {
            struct stat fileStat;
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", direc, filenames[i]);
            if (stat(full_path, &fileStat) < 0) {
                perror("stat");
                continue;
            }

            printf("%c", (S_ISDIR(fileStat.st_mode)) ? 'd' : '-');
            printf("%c", (fileStat.st_mode & S_IRUSR) ? 'r' : '-');
            printf("%c", (fileStat.st_mode & S_IWUSR) ? 'w' : '-');
            printf("%c", (fileStat.st_mode & S_IXUSR) ? 'x' : '-');
            printf("%c", (fileStat.st_mode & S_IRGRP) ? 'r' : '-');
            printf("%c", (fileStat.st_mode & S_IWGRP) ? 'w' : '-');
            printf("%c", (fileStat.st_mode & S_IXGRP) ? 'x' : '-');
            printf("%c", (fileStat.st_mode & S_IROTH) ? 'r' : '-');
            printf("%c", (fileStat.st_mode & S_IWOTH) ? 'w' : '-');
            printf("%c", (fileStat.st_mode & S_IXOTH) ? 'x' : '-');
            printf(" %ld", fileStat.st_nlink);
            printf(" %s", getpwuid(fileStat.st_uid)->pw_name);
            printf(" %s", getgrgid(fileStat.st_gid)->gr_name);
            printf(" %5ld", fileStat.st_size);

            char time_buf[80];
            strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", localtime(&fileStat.st_mtime));
            printf(" %s", time_buf);
            if (S_ISDIR(fileStat.st_mode)) {
                printf(" " BLUE "%s" RESET, filenames[i]);
            } else if (fileStat.st_mode & S_IXUSR) {
                printf(" " GREEN "%s" RESET, filenames[i]);
            } else {
                printf(" " WHITE "%s" RESET, filenames[i]);
            }
            printf("\n");
        } else {
            struct stat fileStat;
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", direc, filenames[i]);
            if (stat(full_path, &fileStat) < 0) {
                perror("stat");
                continue;
            }
            if (S_ISDIR(fileStat.st_mode)) {
                printf(" " BLUE "%-15s" RESET, filenames[i]);
            } else if (fileStat.st_mode & S_IXUSR) {
                printf(" " GREEN "%-15s" RESET, filenames[i]);
            } else {
                printf(" " WHITE "%-15s" RESET, filenames[i]);
            }
            count++;
            if (count >= max_files_per_line) {
                printf("\n");
                count = 0;
            }
        }

        free(filenames[i]);  
    }

    if (!l && count > 0) {
        printf("\n");  
    }
    free(filenames); 
    closedir(dir);
}

void hop_prev_dir(char *cur_dir, char *store_prev_dir, int a, int l){
    // printf("^^^^^ %s\n",store_prev_dir);
    print_files(store_prev_dir, a, l);

}

void hop_parent_dir(char *cur_dir, int a, int l){
    char parent_dir[1024];
    snprintf(parent_dir, sizeof(parent_dir), "..");
    print_files(parent_dir, a, l);
}

void hop_cur_dir(char *cur_dir, int a, int l) {
    print_files(cur_dir, a, l);
    
}

void hop_home_dir(char *shell_home_dir, int a, int l){
    print_files(shell_home_dir, a, l);

}

void hop_path(char *path, int a, int l){
    print_files(path, a, l);

}

void reveal(char **args, int ac, char *shell_home_dir){
    char path[MAX];
    int us = 0;
    bool has_l_flag = false;
    bool has_a_flag = false;
    bool found_path = false;
    //us = 1 ---> previous dir
    //us = 2 ---> parent dir
    //us = 3 ---> current dir
    //us = 4 ---> home dir
    //us = 5 ---> path 
    // printf("Arguments passed to reveal:\n");
    // for (int i = 0; i < ac; i++) {
    //     printf("args[%d]: %s\n", i, args[i]);
    // }

    // printf("ac : %d\n",ac);
    if (ac < 1) {
        if (getcwd(cur_dir, sizeof(cur_dir)) == NULL) {
                printf(RED "Error getting current directory" RESET "\n");
                return;
            }
        hop_cur_dir(cur_dir,has_a_flag,has_l_flag);
    }
    for (int i = 0; i < ac; i++) {
        bool valid_flag = false;
        if (args[i][0] == '-') {
            for (int j = 1; args[i][j] != '\0'; j++) {
                if (args[i][j] == 'l') {
                    has_l_flag = true;
                    valid_flag = true;
                } 
                else if (args[i][j] == 'a') {
                    has_a_flag = true;
                    valid_flag = true;
                } 
                else if(args[i][j] == ' '){
                    us = 1;
                    valid_flag = false;
                }
                else if(args[i][j] == '\n'){
                    us = 1;
                }
            }
            if(args[i][1] == '\0'){
                // printf("$$$$$\n");
                us = 1;
            }
        } 
        // printf("us: %d\n",us);
        // Check if no valid flags were found
        // if (!valid_flag) {
        //     printf("Token \"%s\" doesn't contain any valid flag, it's just '-'\n", args[i]);
        //     us = 1;
        // }
        if ((args[i][0] == '/') || isalpha(args[i][0]) || args[i][0] == '~' || args[i][0] == '.' ) {
            // if(args[i][0] == '-'){
            //     us = 1;
            //     // previous directory
            //     printf("Previous directory\n");
            //     break;

            // }
            if(args[i][0] == '.'){
                if(args[i][1] == '.'){
                    us = 2;
                    // printf("Parent directory\n");
                    break;

                }
                else{
                    us = 3;
                    // printf("Current directory\n");
                    break;
                }
            }
            else if(args[i][0] == '~'){
                us = 4;
                // printf("Home directory\n");
                break;
            }
            else if((args[i][0] == '/')){
                us = 5;
                // printf("Absolute path\n");
                strcpy(path,args[i]);
                break;
            }
            else if(us != 1){
                us = 5;
                found_path = true;
                // printf("Path or symbol: %s\n", args[i]);
                strcpy(path,args[i]);
                break; 
            }  
        }
    }
    // printf("Has l flag: %s\n", has_l_flag ? "Yes" : "No");
    // printf("Has a flag: %s\n", has_a_flag ? "Yes" : "No");

    
    if (us != 0) {
        for (int i = 0; i < ac; i++) {
            if (strcmp(args[i], ".") == 0 || strcmp(args[i], "~") == 0 || strcmp(args[i], "..") == 0) {
                strcpy(path, args[i]);
                break;
            }
        }
    }
    // else{
    //     //display of this current directory
    //     // printf("Displaying current directory\n");
    // }

    struct stat pathStat;
    // if (stat(path, &pathStat) < 0) {
    //     perror("stat");
    //     return;
    // }

    if (S_ISREG(pathStat.st_mode)) {
        // If the path is a file, print its details
        print_file_details(path);
        return;
    }

    if(us == 0 && (has_a_flag || has_l_flag)){
        if (getcwd(cur_dir, sizeof(cur_dir)) == NULL) {
                printf(RED "Error getting current directory" RESET "\n");
                return;
            }
        hop_cur_dir(cur_dir,has_a_flag,has_l_flag);

    }
    // printf("******us : %d\n",us);
    switch(us){
        case 1:
            //previous directory
            if (getcwd(cur_dir, sizeof(cur_dir)) == NULL) {
                printf(RED "Error getting current directory" RESET "\n");
                return;
            }
            // printf("^^^^^^^^^^^\n");
            // printf("store_prev_dir before reveal: %s\n", store_prev_dir);
            hop_prev_dir(cur_dir, store_prev_dir,has_a_flag,has_l_flag);
            break;

        case 2:
            //parent directory
            if (getcwd(cur_dir, sizeof(cur_dir)) == NULL) {
                printf(RED "Error getting current directory" RESET "\n");
                return;
            }
            hop_parent_dir(cur_dir,has_a_flag,has_l_flag);
            break;

        case 3:
            //current directory
            if (getcwd(cur_dir, sizeof(cur_dir)) == NULL) {
                printf(RED "Error getting current directory" RESET "\n");
                return;
            }
            hop_cur_dir(cur_dir,has_a_flag,has_l_flag);
            break;

        case 4:
            //home directory
            hop_home_dir(shell_home_dir,has_a_flag,has_l_flag);
            break;

        case 5:
            //path
            // printf("Path length: %ld\n", strlen(path));
            hop_path(path,has_a_flag,has_l_flag);
            break;
    }
}

    
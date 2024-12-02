#include "headers.h"
int fc = 0;
int dc = 0;
int yes = 0;
char gogo[MAX];

void print_relative_path(const char *absolute_path) {
    char relative_path[MAX];
    if (strncmp(absolute_path, cur_dir, strlen(cur_dir)) == 0) {
        snprintf(relative_path, sizeof(relative_path), "./%s", absolute_path + strlen(cur_dir) + 1);
        printf("%s\n", relative_path);
    } else {
        printf("%s\n", absolute_path);
    }
}
void look(char *target_filename, char *target_path, int d, int e, int f) {
    DIR *dir;
    struct dirent *entry;
    struct stat entry_info;
    char path[4096]; 
    if (!(dir = opendir(target_path))) {
        perror("opendir");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        snprintf(path, sizeof(path), "%s/%s", target_path, entry->d_name);
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (stat(path, &entry_info) == -1) {
            perror("stat");
            continue;
        }

        if (strncmp(entry->d_name, target_filename, strlen(target_filename)) == 0) {
            int is_dir = S_ISDIR(entry_info.st_mode);
            int is_file = S_ISREG(entry_info.st_mode);

            if ((d && is_dir) || (f && is_file) || (!d && !f)) {
                // snprintf(relative_path, sizeof(relative_path), "%s", path + strlen(target_path) + 1);
                if (is_dir) {
                    yes = 1;
                    printf(BLUE);
                    // printf(BLUE "%s\n" RESET, relative_path);
                    print_relative_path(path);
                    printf(RESET);
                    
                } else if (is_file) {
                    yes = 1;
                    printf(GREEN);
                    // printf(GREEN "%s\n" RESET, relative_path);
                    print_relative_path(path);
                    printf(RESET);
                }
            }
        }

        if (S_ISDIR(entry_info.st_mode)) {
            look(target_filename, path, d, e, f);
        }
    }

    closedir(dir);
}


void look_e(char *target_filename, char *target_path, int d, int f) {
    DIR *dir;
    struct dirent *entry;
    struct stat entry_info;
    char path[MAX];


    if (!(dir = opendir(target_path))) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        snprintf(path, sizeof(path), "%s/%s", target_path, entry->d_name);

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (stat(path, &entry_info) == -1) {
            perror("stat");
            continue;
        }
        if (strncmp(entry->d_name, target_filename, strlen(target_filename)) == 0) {
            int is_dir = S_ISDIR(entry_info.st_mode);
            int is_file = S_ISREG(entry_info.st_mode);

            if ((d && is_dir) || (f && is_file) || (!d && !f)) {
                // snprintf(relative_path, sizeof(relative_path), "%s", path + strlen(target_path) + 1);
                if (is_dir) {
                    dc++;
                    yes = 1;
                    strcpy(gogo,path);
                    
                } else if (is_file) {
                    fc++;
                    yes = 1;
                    strcpy(gogo,path);
                }
            }
        }

        if (S_ISDIR(entry_info.st_mode)) {
            look_e(target_filename, path, d, f);
        }
    }

    closedir(dir);
}

void seek_func(char **args, int ac, char *shell_home_dir){
    char path[MAX];
    bool has_f_flag = false;
    bool has_d_flag = false;
    bool has_e_flag = false;
    strcpy(path_seek,"");
    yes = 0;
    if (getcwd(cur_dir, sizeof(cur_dir)) == NULL) {
        printf(RED "Error getting current directory" RESET "\n");
        return;
    }

    for(int i=0; i<ac; i++){
        bool valid_flag = false;
        if(args[i][0] == '-'){
            for (int j = 1; args[i][j] != '\0'; j++) {
                if (args[i][j] == 'f') {
                    has_f_flag = true;
                    valid_flag = true;
                } else if (args[i][j] == 'd') {
                    has_d_flag = true;
                    valid_flag = true;
                }
                else if (args[i][j] == 'e') {
                    has_e_flag = true;
                    valid_flag = true;
                }
            }
        }
        else{
            strcpy(filename_seek,args[i]);
            if(i != ac-1){
                strcpy(path_seek,args[i+1]);
                break;
            }
        }
    }
    if(has_d_flag == 1 && has_f_flag == 1){
        printf(RED "Incorrect flags given for seek\n" RESET);
        return;
    }
    else if(strcmp(path_seek,"") != 0){
        // printf("######\n");
        if(strcmp(path_seek,".") == 0){
            if (getcwd(cur_dir, sizeof(cur_dir)) == NULL) {
                printf(RED "Error getting current directory" RESET "\n");
                return;
            }
            strcpy(path_seek, cur_dir);
            // look(filename_seek, cur_dir, has_d_flag, has_e_flag, has_f_flag);
        }
        else if(strcmp(path_seek,"~") == 0){
            strcpy(path_seek, shell_home_dir);
            // look(filename_seek, shell_home_dir, has_d_flag, has_e_flag, has_f_flag);
        }
        else if(strcmp(path_seek,"..") == 0){
            char parent_dir[1024];
            snprintf(parent_dir, sizeof(parent_dir), "..");
            strcpy(path_seek, parent_dir);
            // look(filename_seek, parent_dir, has_d_flag, has_e_flag, has_f_flag);

        }
        else if(strcmp(path_seek,"-") == 0){
            strcpy(path_seek,store_prev_dir);
            // look(filename_seek, store_prev_dir, has_d_flag, has_e_flag, has_f_flag);
        }  
    }
    else{
        // printf("$$$$$\n");
        strcpy(path_seek, cur_dir);
    }

    // if(has_f_flag){
    //     printf("f\n");
    // }
    // if(has_d_flag){
    //     printf("d\n");
    // }
    // if(has_e_flag){
    //     printf("e\n");
    // }

    if(!has_e_flag){
        // printf("^^^^^^\n");
        if(has_d_flag && has_f_flag){
            printf(RED "Invalid flags!\n" RESET);
        }
        else{
            look(filename_seek, path_seek, has_d_flag, has_e_flag, has_f_flag);
            if(yes != 1){
                printf(GRAY "No match found!\n" RESET);
            }
        }
    }
    else{
        // printf("$$$$\n");
        fc = 0;
        dc = 0;
        if(has_d_flag && has_f_flag){
            printf(RED "Invalid flags!\n" RESET);
            return;
        }
        else if(!has_d_flag && !has_f_flag){
            look_e(filename_seek, path_seek, has_d_flag, has_f_flag);
            if(yes != 1){
                printf(GRAY "No match found!\n" RESET);
                return;
            }
            // printf("fc and dc : %d, %d\n",fc, dc);
            if(fc == 1 && dc == 0){
                //file output
                // printf("path : %s\n",path);
                printf("gogo : %s\n",gogo);
                int fd = open(gogo,O_RDONLY);
                if(fd == -1){
                    perror("open() error");
                    return;
                }
                char buffer[4096];
                ssize_t bytes_read;
                while((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
                    buffer[bytes_read] = '\0';  
                    printf(GREEN "%s" RESET, buffer);
                }
                printf("\n");

                if(bytes_read == -1) {
                    perror("read() error");
                }
                close(fd);
            }
            else if(fc == 0 && dc == 1){
                //change dir
                if(chdir(gogo) == -1) {
                    perror("chdir");
                } 
                // else {
                //     printf(BLUE "Changed directory to: %s\n" RESET, gogo);
                // }
            }
            else{
                look(filename_seek, path_seek, has_d_flag, has_e_flag, has_f_flag);
                if(yes != 1){
                    printf(GRAY "No match found!\n" RESET);
                }
            }
        }
        else if(has_d_flag == 1 || has_f_flag == 1){
            look_e(filename_seek, path_seek, has_d_flag, has_f_flag);
            if(yes != 1){
                printf(GRAY "No match found!\n" RESET);
                return;
            }
            // printf("fc and dc : %d, %d\n",fc, dc);
            if(has_d_flag){
                if(dc == 1){
                    //change dir
                    if(chdir(gogo) == -1) {
                        perror("chdir");
                    } 
                    // else {
                    //     printf(BLUE "Changed directory to: %s\n" RESET, gogo);
                    // }
                }
                else{
                    look(filename_seek, path_seek, has_d_flag, has_e_flag, has_f_flag);
                    if(yes != 1){
                        printf(GRAY "No match found!\n" RESET);
                        return;
                    }
                }
            }
            else if(has_f_flag){
                if(fc == 1){
                    //output file
                    int fd = open(gogo,O_RDONLY);
                    if(fd == -1){
                        perror("open() error");
                        return;
                    }
                    char buffer[4096];
                    ssize_t bytes_read;
                    while((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
                        buffer[bytes_read] = '\0'; 
                        printf(GREEN "%s" RESET, buffer);
                    }
                    printf("\n");

                    if(bytes_read == -1) {
                        perror("read() error");
                    }
                    close(fd);
                }
                else{
                    look(filename_seek, path_seek, has_d_flag, has_e_flag, has_f_flag);
                    if(yes != 1){
                        printf(GRAY "No match found!\n" RESET);
                        return;
                    }
                }
            }
        }
    }
}
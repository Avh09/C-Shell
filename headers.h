#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <termios.h>

#include "directory.h"
#include "colors.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "token.h"
#include "proclore.h"
#include "seek.h"
#include "io.h" 
#include "pipes.h"
#include "activities.h"
#include "iman.h"
#include "signals.h"
#include "fgbg.h"
#include "alias.h"
#include "neo.h"
#include "background.h"

#define LIMIT 4096
#define MAX 4096
#define MAX_LINE_LENGTH 1024 
#define MAX_LINES 15    
#define PORT 80
#define BUFFER_SIZE 1000000
#define HASH_SIZE 256

extern char process_info[512];
extern char last_long_running_command[512];
extern char prev_dir[MAX];      
extern char cur_dir[MAX];                   
extern char new_cur_dir[MAX];               
extern char full_path[MAX];
extern char store_prev_dir[MAX];
extern char shell_home_dir[MAX];
extern char filepath[MAX];
extern char filename_seek[MAX];
extern char path_seek[MAX];
extern char path[MAX];
extern char relative_path[MAX];
extern char io_filepath[MAX];
extern char io_filename[MAX];
extern int is_redirected;
extern char copy_command[MAX];
extern int sig_flag;
extern pid_t running_processes[MAX];
extern int process_count;
extern int neo_flag;



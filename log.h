#ifndef __LOG_H__
#define __LOG_H__
#include "headers.h"
void shift_lines(char lines[MAX_LINES][MAX_LINE_LENGTH], int line_count);
void write_to_log_file(char *command);
void write_lines_to_file(char *filename, char lines[MAX_LINES][MAX_LINE_LENGTH], int line_count);
void read_lines_from_file(char *filename, char lines[MAX_LINES][MAX_LINE_LENGTH], int *line_count);
void log_thing(char *command);  
void log_func(); 
void purge();
void log_execute(int val);
#endif
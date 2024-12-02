#ifndef __IMAN_H__
#define __IMAN_H__
#include "headers.h"

void iman(char *user_input);
void build_http_request(char *buffer, const char *hostname, const char *path);

#endif
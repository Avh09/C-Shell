#include "headers.h"

void build_http_request(char *buffer, const char *hostname, const char *path) {
    snprintf(buffer, BUFFER_SIZE, 
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n", 
             path, hostname);
}

void iman(char *user_input) {
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE];
    char request[BUFFER_SIZE];
    char mylink[MAX];
    char hostname[] = "man.he.net";
    strcpy(mylink, "/?topic=");  

    FILE *output_file;

    strcat(mylink, user_input);
    strcat(mylink, "&section=all");

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return;
    }

    if ((server = gethostbyname(hostname)) == NULL) {
        perror("Host not found");
        close(sockfd);
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        return;
    }

    build_http_request(request, hostname, mylink);

    if (send(sockfd, request, strlen(request), 0) < 0) {
        perror("Send failed");
        close(sockfd);
        return;
    }

    output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        perror("File opening failed");
        close(sockfd);
        return;
    }

    int bytes_received;
    int in_pre_tag = 0; 

    while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';  
        
        char *pre_start = strstr(buffer, "<PRE>");
        char *pre_end = strstr(buffer, "</PRE>");

        if (pre_start) {
            pre_start += 5; 
            in_pre_tag = 1;
        }

        if (in_pre_tag) {
            if (pre_end) {
                *pre_end = '\0';
                fputs(pre_start, output_file);  
                break;  
            } else {
                fputs(pre_start, output_file); 
            }
        }
    }

    if (bytes_received < 0) {
        perror("Receive failed");
    }

    close(sockfd);

    fclose(output_file);

    output_file = fopen("output.txt", "r");
    if (output_file == NULL) {
        perror("File reading failed");
        return;
    }

    while (fgets(buffer, sizeof(buffer), output_file) != NULL) {
        printf("%s", buffer);
    }
    fclose(output_file);
}

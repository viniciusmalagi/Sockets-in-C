#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#define PORT 5000
#define BUFFER_SIZE 1024

int main(){
    struct sockaddr_in server_addr;
    int server_fd, new_fd;
    int opt = 1;
    int server_len = sizeof(server_addr);
    char buff[BUFFER_SIZE];
    char* exit = "exit";

    server_addr.sin_family = AF_INET,
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY),
    server_addr.sin_port = htons(PORT);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("server socket failed.");
        return -1;
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        printf("setsockopt failed");
        return -1;
    }
    if (bind(server_fd, (struct sockaddr *)&server_addr, server_len) < 0){
        printf("bind failed.");
        return -1;
    }
    if (listen(server_fd, 1) < 0){
        printf("listen failed.");
        return -1;
    }
    printf("Waiting client...\n");
    if ((new_fd = accept(server_fd, (struct sockaddr*)&server_addr, &server_len)) < 0){
        printf("client socket failed.");
        return -1;
    }
    printf("[CONNECTED]\n");
    while(1){
        bzero(buff, BUFFER_SIZE);
        read(new_fd, buff, BUFFER_SIZE);
        printf("\n [Client] : %s\n", buff);
        bzero(buff, BUFFER_SIZE);
        fgets(buff, BUFFER_SIZE, stdin);
        write(new_fd, buff, BUFFER_SIZE);
        printf("\n [You] : %s\n", buff);
        if (!strncmp(buff, exit, strlen(exit))){
            break;
        }
    }
    close(new_fd);
	shutdown(server_fd, SHUT_RDWR);
    printf("[CLOSED]\n");
    return 0;
}
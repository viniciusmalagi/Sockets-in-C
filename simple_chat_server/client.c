#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#define PORT 5000
#define BUFFER_SIZE 1024

int main(){
    struct sockaddr_in address;
    int client_fd, status;
    char buff[BUFFER_SIZE];
    char* exit = "exit";

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Socket failed\n");
        return -1;
    }
    if ((status = connect(client_fd, (struct sockaddr*)&address, sizeof(address))) < 0){
        printf("\n Connection failed. \n");
        return -1;
    }
    printf("[Connected]\n");
    while(1){
        bzero(buff, sizeof(buff));
        fgets(buff, BUFFER_SIZE, stdin);
        write(client_fd, buff, strlen(buff));
        printf("\n [You] : %s\n", buff);
        if (!strncmp(buff, exit, strlen(exit))){
            break;
        }
        bzero(buff, BUFFER_SIZE);
        read(client_fd, buff, BUFFER_SIZE);
        printf("\n[Server] : %s\n", buff);
    }
    close(client_fd);
    printf("[CLOSED]\n");
    return 0;
}
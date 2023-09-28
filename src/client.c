#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 4444
#define BUFFER_SIZE 1024


int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to server
    if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    
    
    if(fgets(buffer, sizeof(buffer), stdin) != NULL) {
        size_t buffer_length = strlen(buffer);
        buffer[buffer_length - 1] = '\0';
    }
    
    if(send(client_socket, buffer, strlen(buffer), 0) == -1) {
        perror("Error sending data");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    
    // Receive data from server

    close(client_socket);
    return 0;
}
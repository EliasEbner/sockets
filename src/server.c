#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 4444
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    char buffer[BUFFER_SIZE] = "hello, client!";

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        exit(EXIT_FAILURE);
    }
    
    if(listen(server_socket, 2) == -1) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
    if(client_socket == -1) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    printf("Connected to client %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if(bytes_received == -1) {
        perror("Error receiving data");
        close(server_socket);
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    
    buffer[bytes_received] = '\0';
    
    printf("Received message from client: %s\n", buffer);

    close(client_socket);
    close(server_socket);

    return 0;
}
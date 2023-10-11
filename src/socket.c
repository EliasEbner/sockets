#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "socket.h"

// |---------------------------|
// |        SERVER CODE        |
// |---------------------------|

int max_clients;
int num_clients = 0;


// the thread that is created for each client
void* client_thread(void* current_client_pointer) {
  int current_client = *((int *)current_client_pointer);
  while(1) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    
    if(recv(current_client, buffer, sizeof(buffer), 0) == 0) {
      printf("Client disconnected.\n");
      return NULL;
    }
        
    printf("%s", buffer);
  }
  return NULL;
}


// initialize the server
int init_server(int port, int max_clients_local) {
  // configure client handling
  max_clients = max_clients_local;
  clients = malloc(sizeof(client) * max_clients);

  // create server socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(server_socket < 0) {
    perror("Server socket creation failed.");
    exit(1);
  }

  // configure server
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  
  // bind
  if(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Server binding failed.");
    exit(1);
  }
  
  // listen
  if(listen(server_socket, max_clients) == 0) {
    printf("Server is listening on port %d... \n", port);
  } else {
    perror("Server listening failed.");
    exit(1);
  }
  
  // init mutex
  if(pthread_mutex_init(&lock, NULL) != 0) {
    perror("Mutex initialization failed.");
    exit(1);
  };
  
  return server_socket;
}


// handle new connections and create new thread for each client
void accept_clients(int server_socket) {
  // stop accepting connections when there are too many clients 
  while(num_clients < max_clients) {
    // initialize client
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);

    // accept client connection
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
    if(client_socket < 0) {
      perror("Accepting client connection failed.");
      continue;
    }

    // add client to list (using mutex)
    pthread_mutex_lock(&lock);
    clients[num_clients].socket = client_socket;
    // create client thread
    pthread_create(&(clients[num_clients].thread), NULL, client_thread, &clients[num_clients].socket);
    num_clients++;
    pthread_mutex_unlock(&lock);
    printf("New client connected. Total clients: %d\n", num_clients);
  }
}

// close server socket
void server_close(int server_socket) {
  close(server_socket);
}



// |---------------------------|
// |        CLIENT CODE        |
// |---------------------------|

int init_client(char* server_ip, int port) {
  // create client socket
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(client_socket < 0) {
    perror("Socket creation failed.");
    exit(1);
  }
  
  // configure server
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(server_ip);
  server_addr.sin_port = htons(port);
  
  // connect to server
  if(connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Connection to server failed.");
    exit(1);
  }
  
  return client_socket;
}

// send data to server
void client_send(int client_socket, char* data) {
  send(client_socket, data, strlen(data), 0);
}

// recv data from server
char* client_recv(int client_socket, char* buffer) {
  // recv the data
  ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
  if(bytes_received == -1) {
    perror("Data reception failed.");
    exit(1);
  } else if (bytes_received == 0) {
    printf("Server closed the connection.\n");
    return NULL;
  }

  // null-terminate buffer and return it
  buffer[bytes_received] = '\0';
  return buffer;
}

// close the socket
void client_close(int client_socket) {
  close(client_socket);
}


/*
char* message[64];
if(fgets(message, sizeof(message), stdin) != NULL) {
  size_t len = strlen(message);
  if(len > 0 && message[len - 1] == '\n') {
    message[len - 1] = '\0';
  }
}
*/
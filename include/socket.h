#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>



pthread_mutex_t lock;

typedef struct {
  pthread_t thread;
  int socket;
} client;

client* clients;



int init_server(int port, int maxClients);
void accept_clients();



int init_client(char* server_ip, int port);
void client_send(int client_socket, char* data);
char* client_recv(int client_socket, char* buffer);
void client_close(int client_socket);

#endif

#include "socket.h"


int main() {
    int server_socket = init_server(8080, 5);
    accept_clients(server_socket);
    return 0;
}
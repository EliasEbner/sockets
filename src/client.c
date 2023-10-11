#include "socket.h"

int main() {
    int client_socket = init_client("127.0.0.1", 8080);
    client_send(client_socket, "hello");
    client_close(client_socket);
    return 0;
}

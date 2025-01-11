#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT "8080"
#define BACKLOG 10
#define BUFFER_SIZE 1024

/*
    * define a port to listen
    * add the local ip address 
    * 
*/

int main()
{
    int                     fd_sock_server, fd_client, ret_value = { 0 };
    struct addrinfo         hints;
    struct addrinfo         *res;
    struct sockaddr_storage client_addr;
    socklen_t               client_addr_size;

    hints.ai_family = AF_UNSPEC;        // addr ipv4 or ipv6
    hints.ai_socktype = SOCK_STREAM;    // connexion type, here TCP (Transmission Control Protocol)
    hints.ai_flags = AI_PASSIVE;        // auto-populated ip field

    ret_value = getaddrinfo(NULL, PORT, &hints, &res);
    if (ret_value != 0) {
        std::cerr << gai_strerror(ret_value) << std::endl;
        return (1);
    }

    fd_sock_server = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd_sock_server < 0) {
        std::cerr << "Error openning socket" << std::endl;
        freeaddrinfo(res);
        return (2);
    }

    if (bind(fd_sock_server, res->ai_addr, res->ai_addrlen) != 0) {
        std::cerr << "Bind impossibe" << std::endl;
        freeaddrinfo(res);
        return (3);
    }

    if (listen(fd_sock_server, BACKLOG) != 0) {
        std::cerr << "Error listening port" << std::endl;
        freeaddrinfo(res);
        return (4);
    }

    client_addr_size = sizeof(client_addr);
    fd_client = accept(fd_sock_server, (struct sockaddr *) &client_addr, &client_addr_size);
    if (fd_client < 0) {
        std::cerr << "Error accept client" << std::endl;
        freeaddrinfo(res);
    }
    freeaddrinfo(res);

    return (0);
}

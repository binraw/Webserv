// #include <iostream>
// #include <sstream>
// #include <string>
// #include <cstring>

// #include <unistd.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <arpa/inet.h>

// #define SERVICE "8080"
// #define BACKLOG 10
// #define BUFFER_SIZE 1024

/*
    * define a port to listen
    * add the local ip address 
*/



// int main()
// {
    // char *port = server.config.at(port);
    // int                     fd_sock_server, fd_client, ret_value, bytes_received = { 0 };
    // struct addrinfo         hints;
    // struct addrinfo         *res;
    // struct sockaddr_storage client_addr;
    // socklen_t               client_addr_size;
	// char					buffer[BUFFER_SIZE];

    // memset(&hints, 0, sizeof(hints));
    // hints.ai_family = AF_UNSPEC;        // addr ipv4 or ipv6
    // hints.ai_socktype = SOCK_STREAM;    // connexion type, here TCP (Transmission Control Protocol)
    // hints.ai_flags = AI_PASSIVE;        // auto-populated ip field

    // ret_value = getaddrinfo(NULL, SERVICE, &hints, &res);
    // if (ret_value != 0) {
    //     std::cerr << "Here: " << gai_strerror(ret_value) << std::endl;
    //     return (1);
    // }

    // fd_sock_server = socket(res->ai_family, res->ai_socktype, res->ai_protocol);	// init socket with hints params
    // if (fd_sock_server < 0) {                                                   	
    //     std::cerr << "Error openning socket" << std::endl;
    //     freeaddrinfo(res);
    //     return (2);
    // }

    // if (bind(fd_sock_server, res->ai_addr, res->ai_addrlen) != 0) // lie le socket a un port et une adress
    // {
    //     std::cerr << "Bind impossibe" << std::endl;
    //     freeaddrinfo(res);
    //     return (3);
    // }

    // if (listen(fd_sock_server, BACKLOG) != 0) // socket en mode écoute pour les connexions entrantes, spécifiant le nombre maximum de connexions en attente.
    // {
    //     std::cerr << "Error listening port" << std::endl;
    //     freeaddrinfo(res);
    //     return (4);
    // }

    // client_addr_size = sizeof(client_addr);
    // fd_client = accept(fd_sock_server, (struct sockaddr *) &client_addr, &client_addr_size); // accepte la connexion et creer un nouveau socket
    // if (fd_client < 0) {
    //     std::cerr << "Error accept client" << std::endl;
    //     freeaddrinfo(res);
    // }

	// memset(buffer, '\0', sizeof(buffer));
	// bytes_received = recv(fd_client, buffer, sizeof(buffer) - 1, 0);
	
	// if (bytes_received < 0) 
    // {
	// 	std::cerr << "Erreur lors de la réception des données" << std::endl;
	// 	close(fd_client);
	// 	freeaddrinfo(res);
	// 	return (5);
	// }
	// const char *http_response =
    // "HTTP/1.1 200 OK\r\n"
    // "Content-Type: text/html\r\n"
    // "Content-Length: 13\r\n"
    // "\r\n"
    // "Hello, World!";

	// if (send(fd_client, http_response, strlen(http_response), 0) < 0) 
    // {
	// 	std::cerr << "Erreur lors de l'envoi de la réponse" << std::endl;
	// 	close(fd_client);
	// 	freeaddrinfo(res);
	// 	return (6);
	// }

	// close(fd_client);
	// std::cout << "New client : " << client_addr.ss_family << std::endl;
	// close(fd_sock_server);
    // freeaddrinfo(res);

//     return (0);
// }



#include "Server.hpp"

int main(void)
{
    Server one("server.conf");

    one.init_data();
    one.controlMap();
    
    return 0;
}

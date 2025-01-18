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

// /*
//     * define a port to listen
//     * add the local ip address 
// */

// #include "webserv.hpp"


// int main()
// {
//     // char *port = server.config.at(port);
//     int                     fd_sock_server, fd_client, ret_value, bytes_received = { 0 };
//     struct addrinfo         hints;
//     struct addrinfo         *res;
//     struct sockaddr_storage client_addr;
//     socklen_t               client_addr_size;
// 	char					buffer[BUFFER_SIZE];

//     struct protoent *protoName = getprotobyname("tcp");
//     if (protoName == NULL) {
//         std::cerr   << RED ERROR
//                     << "getprotobyname returned error at line [" << __LINE__ << "] in " << __FILE__
//                     << RESET << std::endl;
//         return 1;
//     }

//     memset(&hints, 0, sizeof(hints));
//     hints.ai_family = AF_UNSPEC;        // addr ipv4 or ipv6
//     hints.ai_socktype = SOCK_STREAM;    // connexion type, here TCP (Transmission Control Protocol)
//     hints.ai_flags = AI_PASSIVE;        // auto-populated ip field

//     ret_value = getaddrinfo("exemple.com", SERVICE, &hints, &res);
//     if (ret_value != 0) {
//         std::cerr << "Here: " << gai_strerror(ret_value) << std::endl;
//         return (1);
//     }

//     fd_sock_server = socket(res->ai_family, res->ai_socktype, res->ai_protocol);	// init socket with hints params
//     if (fd_sock_server < 0) {                                                   	
//         std::cerr << "Error openning socket" << std::endl;
//         freeaddrinfo(res);
//         return (2);
//     }


//     return (0);
// }

// // #include "Server.hpp"

// // int main(void)
// // {
// //     Server one("server.conf");

// //     one.init_data();
// //     one.controlMap();
    
// //     return 0;
// // }

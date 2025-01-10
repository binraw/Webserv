// #include <iostream>
// #include <sstream>
// #include <string>
// #include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
 
// int main()
// {
//     int server_fd, client_socket;
//     struct sockaddr_in address;
//     int opt = 1;
//     int addrlen = sizeof(address);
//     char buffer[BUFFER_SIZE] = {0};

//     // Création du socket
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
//     {
//         perror("Erreur de création du socket");
//         exit(EXIT_FAILURE);
//     }
 
//     // Attachement du socket au port
//     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) 
//     {
//         perror("Erreur lors de l'attachement du socket");
//         exit(EXIT_FAILURE);
//     }
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_LOOPBACK; // Accepte des connexions de n'importe quelle adresse IP
//     address.sin_port = htons(PORT);
 
//     // Liaison du socket à l'adresse
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
//     {
//         perror("Erreur de liaison");
//         exit(EXIT_FAILURE);
//     }
 
//     // Écoute des connexions entrantes
//     if (listen(server_fd, 3) < 0)
//     { // ici le 3 est le nombre max attente de connection
//         perror("Erreur lors de l'écoute");
//         exit(EXIT_FAILURE);
//     }
 
//     std::cout << "Serveur en écoute sur le port " << PORT << "..." << std::endl;
 
//     // Boucle principale pour accepter les connexions
//     while (true)
//     {
//         if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) 
//         {
//             perror("Erreur lors de l'acceptation de la connexion");
//             exit(EXIT_FAILURE);
//         }
 
//         // Lire la requête du client
//         read(client_socket, buffer, BUFFER_SIZE);
//         std::cout << "Requête reçue:\n" << buffer << std::endl;
 
//         // Répondre au client avec un message HTTP simple
//         std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><body><h1>Hello, World!</h1></body></html>";
//         send(client_socket, response.c_str(), response.size(), 0);
 
//         // Fermer le socket de la connexion
//         close(client_socket);
//     }
 
//     return 0;
// }

// server.c - un micro-serveur qui accepte une connexion avant de s'arrêter
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

// #define PORT 4242  // le port de notre serveur
#define BACKLOG 10 // nombre max de demandes de connexion dans la file d'attente

int main(void)
{
    struct sockaddr_in sa;
    int socket_fd;
    int client_fd;
    int status;
    // sockaddr_storage est une structure qui n'est pas associé à
    // une famille particulière. Cela nous permet de récupérer
    // une adresse IPv4 ou IPv6
    struct sockaddr_storage client_addr;
    socklen_t addr_size;

    // on prépare l'adresse et le port pour la socket de notre serveur
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET; // IPv4; utiliser AF_INET6 pour IPv6
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1, localhost
    sa.sin_port = htons(PORT);

    // on crée la socket, on a lie et on écoute dessus
    socket_fd = socket(sa.sin_family, SOCK_STREAM, 0);
    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status != 0) {
        fprintf(stderr, "bind: %s\n", strerror(errno));
        return (2);
    }
    listen(socket_fd, BACKLOG);

    // on accept une connexion entrante
    addr_size = sizeof client_addr;
    client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_size);
    if (client_fd == -1) {
        fprintf(stderr, "accept: %s\n", strerror(errno));
        return (3);
    }
    printf("New connection! Socket fd: %d, client fd: %d\n", socket_fd, client_fd);

    // on est prêts à communiquer avec le client via le client_fd !

    return (0);
}

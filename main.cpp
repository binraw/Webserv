#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Création du socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Erreur de création du socket");
        exit(EXIT_FAILURE);
    }

    // Attachement du socket au port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Erreur lors de l'attachement du socket");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Accepte des connexions de n'importe quelle adresse IP
    address.sin_port = htons(PORT);

    // Liaison du socket à l'adresse
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erreur de liaison");
        exit(EXIT_FAILURE);
    }

    // Écoute des connexions entrantes
    if (listen(server_fd, 3) < 0) {
        perror("Erreur lors de l'écoute");
        exit(EXIT_FAILURE);
    }

    std::cout << "Serveur en écoute sur le port " << PORT << "..." << std::endl;

    // Boucle principale pour accepter les connexions
    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) 
        {
            perror("Erreur lors de l'acceptation de la connexion");
            exit(EXIT_FAILURE);
        }

        // Lire la requête du client
        read(new_socket, buffer, BUFFER_SIZE);
        std::cout << "Requête reçue:\n" << buffer << std::endl;

        // Répondre au client avec un message HTTP simple
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><body><h1>Hello, World!</h1></body></html>";
        send(new_socket, response.c_str(), response.size(), 0);

        // Fermer le socket de la connexion
        close(new_socket);
    }

    return 0;
}

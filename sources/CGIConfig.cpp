#include "CGIConfig.hpp"

CGIConfig::CGIConfig()
{
}

CGIConfig::~CGIConfig()
{
}

void CGIConfig::handleRequest(int clientSocket)
{
    char buffer[1024];
    int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
    buffer[bytesRead] = '\0';
    std::string request = std::string(buffer);

    if (request.find("GET /cgi-bin/") != std::string::npos)
    {
        //ici extraire le chemin et traiter la requete CGi
    }
    else
    {
        //ici traiter la requete normalement
    }
    close(clientSocket);
}
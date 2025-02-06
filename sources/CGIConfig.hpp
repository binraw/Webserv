#ifndef CGICONFIG_HPP
# define CGICONFIG_HPP

#include <unistd.h>
#include <string>

class CGIConfig
{
private:
    
public:
    CGIConfig();
    ~CGIConfig();
    void handleRequest(int clientSocket);
};


// ici je vais lire les requetes pour definir si c'est une CGi attendu


#endif
#ifndef ARequest_HPP
# define ARequest_HPP

#include <unistd.h>
#include <string>
#include "../includes/webserv.hpp"

class ARequest
{
    protected:
        std::string _pathToFile;
private:
     ARequest(){};
public:
    virtual ~ARequest();
    virtual void handle() = 0;

    
};


ARequest* handleRequest(int clientSocket);

// ici je vais lire les requetes pour definir si c'est une CGi attendu


#endif

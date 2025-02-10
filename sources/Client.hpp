#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include "ARequest.hpp"
#include "Server.hpp"
#include <sys/wait.h>

class Client /*: virtual public Server*/
{
private:
    // les valeurs suivantes ne sont pas sur a voir avec le fonctionnement de la page html
    std::string _nameFileTeleverser;
    std::string _contentFileTeleverser;


    std::string _pathCGI;
    
public:
    // Client(int fd, ARequest* request);
    std::string playCGI();
    Client(std::string file);
    std::string playUploadCGI();
    ~Client();
};





#endif

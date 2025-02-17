#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include "ARequest.hpp"
#include "Server.hpp"
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

class Client /*: virtual public Server*/
{
private:
    // les valeurs suivantes sont la pour utiliser la fct Upload

    std::string _nameFileTeleverser;
    std::string _contentFileTeleverser;


    std::string _pathCGI;
    
public:
    // Client(int fd, ARequest* request);
    ARequest* request;
    
    
    std::string response;
    std::string playCGI();
    Client(std::string file);
    std::string playUploadCGI();
    ~Client();
    Client &operator=(const Client &);
};





#endif

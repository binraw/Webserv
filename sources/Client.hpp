#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include "ARequest.hpp"
#include "Server.hpp"
#include <sys/wait.h>

class Client : virtual public Server
{
private:
    int _fd;
    ARequest* _request;
    // variables utils a tout typoes de requete
     
    
    std::string _pathCGI;
    std::string playCGI();
public:
    // Client(int fd, ARequest* request);
    Client();
    ~Client();
};





#endif
#ifndef SERVER_HPP
# define SERVER_HPP

#include <netdb.h>
#include <string>
#include "AServer.hpp"

class Server : virtual public AServer
{
    public:
        Server();
        ~Server();
    
    private:
        protoent    * _protocolInfo;
        protoent    * setProtcolInfo(const std::string & proto)
                    throw(ClusterException); // appele dans constructeur ?


        std::string & _name;
        int         * _sockFd;

};

#endif

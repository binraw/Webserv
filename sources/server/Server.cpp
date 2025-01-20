



#include "Server.hpp"


Server::Server(const std::string& filename)
{    }

Server::~Server()
{
}

protoent    * Server::setProtcolInfo(const std::string & proto)
{
    this->_protocolInfo = getprotobyname(proto.c_str());
    if (_protocolInfo == NULL) {
        std::cerr   << RED
                    << "function: getprotobyname()"
                    << RESET << std::endl;
        throw ClusterException().what();
    }
    return this->_protocolInfo;
}

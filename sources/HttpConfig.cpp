#include "HttpConfig.hpp"

std::vector<std::string> HttpConfig::getIncludes()
{
    return (_include);
}
std::string HttpConfig::getDefaultType()
{
    return (_default_type);
}
std::string HttpConfig::getKeepalive()
{
    return (_keepalive_timeout);
}
std::string HttpConfig::getWorkerConnexion()
{
    return (_worker_connexion);
}

void HttpConfig::displayDefaultHttp()
{
    for (std::vector<std::string>::iterator it = _include.begin(); it != _include.end(); it++)
        std::cout << *it << " " << std::endl;
    std::cout <<  "Default type Cluster: " << getDefaultType() <<  " Keepalive: " << getKeepalive() <<
        " Worker Connexion: " << getWorkerConnexion() << std::endl; 
}


void HttpConfig::displayServers()
{
    for(std::vector<ServerConfig>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        it->displayValueServer();
    }
}

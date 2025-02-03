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
    std::cout <<  getDefaultType() << std::endl;
    std::cout <<  getKeepalive() << std::endl;
    std::cout << getWorkerConnexion() << std::endl; 
}


void HttpConfig::displayServers()
{
    for(std::vector<ServerConfig>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        it->displayValueServer();
    }
}

void HttpConfig::controlDefaultHttpConf()
{
    if (_include.empty())
        throw std::invalid_argument("'Include' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
    if (_default_type.empty())
        throw std::invalid_argument("'default_type' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
    if (_keepalive_timeout.empty())
        throw std::invalid_argument("'keepalive_timeout' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
    if (_worker_connexion.empty())
        throw std::invalid_argument("'worker_connexion' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");

    for (size_t i = 0; i <= _servers.size() - 1; i++)
    {
        _servers[i].controlDefaultServerConf();
        for (std::vector<LocationConfig>::iterator it = _servers[i]._locations.begin(); it != _servers[i]._locations.end(); it++)
            it->controlDefaultLocationConf();
    }
}

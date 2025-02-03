#include "ServerConfig.hpp"

void ServerConfig::displayValueServer()
{
    std::cout <<  _clientMaxBodySize << std::endl;
    std::cout <<  _uploadPath << std::endl;
    UtilParsing::displayVector(_serverName);
    UtilParsing::displayVector(_listenPort);
    displayAllLocations();
}




void ServerConfig::displayAllLocations()
{
    for(std::vector<LocationConfig>::iterator it = _locations.begin(); it != _locations.end(); it++)
    {
        it->displayLocation();
    }
}

void ServerConfig::controlDefaultServerConf()
{
    if (_serverName.empty())
        throw std::invalid_argument("'server_name' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
    if (_listenPort.empty())
        throw std::invalid_argument("'listen' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
    if (_clientMaxBodySize.empty())
        throw std::invalid_argument("'client_max_body_size' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
    if (_uploadPath.empty())
        throw std::invalid_argument("'upload_path' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
    if (_locations.empty())
        throw std::invalid_argument("'location' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");

}

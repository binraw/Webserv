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
void ServerConfig::checkSemiColonServer()
{
    if (_clientMaxBodySize.find(";") != std::string::npos)
        _clientMaxBodySize = UtilParsing::trimSemicolon(_clientMaxBodySize);
    if (_uploadPath.find(";") != std::string::npos)
        _uploadPath =  UtilParsing::trimSemicolon(_uploadPath);
    for (std::vector<std::string>::iterator it = _listenPort.begin(); it != _listenPort.end(); it++)
    {
        if (it->find(";") != std::string::npos)
            *it = UtilParsing::trimSemicolon(*it);
    }
    for (std::vector<std::string>::iterator i = _serverName.begin(); i != _serverName.end(); i++)
    {
        if (i->find(";") != std::string::npos)
            *i = UtilParsing::trimSemicolon(*i);
    }
}

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP


#include "LocationConfig.hpp"
#include "UtilParsing.hpp"


class ServerConfig 
{
public:
    std::vector<std::string>  _serverName;
    std::vector<std::string> _listenPort;
    std::string _clientMaxBodySize;
    std::string _uploadPath;
    std::vector<LocationConfig> _locations;

    void displayValueServer();
    void displayAllLocations();
};

#endif

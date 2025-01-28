#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP


#include "ConfigParser.hpp"
#include "LocationConfig.hpp"


class ServerConfig 
{
public:
    std::vector<std::string>  serverName;
    std::vector<std::string> listenPort;
    std::string clientMaxBodySize;
    std::string uploadPath;
    std::vector<LocationConfig> locations;
};

#endif

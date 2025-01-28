#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP

#include "ConfigParser.hpp"
#include "ServerConfig.hpp"

class HttpConfig {
public:
    std::vector<ServerConfig> servers;
};

#endif

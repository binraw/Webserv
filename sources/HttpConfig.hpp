#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP


#include "ServerConfig.hpp"

class HttpConfig 
{
public:
    std::vector<std::string> _include;
    std::string _default_type;
    std::string _keepalive_timeout;
    std::string _worker_connexion;
    std::vector<ServerConfig> _servers;


    std::vector<std::string> getIncludes();
    std::string getDefaultType();
    std::string getKeepalive();
    std::string getWorkerConnexion();
    void displayDefaultHttp();
    void displayServers();
    void controlDefaultHttpConf();
    void checkSemiColonAllValues();


};

#endif

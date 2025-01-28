#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

#include <vector>
#include "UtilParsing.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <unistd.h>
#include "HttpConfig.hpp"
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"

class ConfigParser 
{
public:
    std::vector<std::string> _include;
    std::string _default_type;
    std::string _keepalive_timeout;
    std::string _worker_connexion;
    HttpConfig parse(const std::string& filepath);

private:
    void displayDefaultConf();
    void parseHttpBlock(std::ifstream& file, HttpConfig& httpConfig);
    void parseServerBlock(std::ifstream& file, ServerConfig& serverConfig);
    void parseLocationBlock(std::ifstream& file, LocationConfig& locationConfig);
};


/* include ./error_pages ./mime.types;
    default_type application/octet-stream;
    keepalive_timeout 65;
    worker_connexion 1024;*/
#endif

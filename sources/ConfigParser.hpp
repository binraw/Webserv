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


class ConfigParser 
{
public:
    HttpConfig parse(const std::string& filepath);
    void controlStructFile(std::ifstream& file);
private:
    void parseHttpBlock(std::ifstream& file, HttpConfig& httpConfig);
    void parseServerBlock(std::ifstream& file, ServerConfig& serverConfig);
    void parseLocationBlock(std::ifstream& file, LocationConfig& locationConfig);
    
};


#endif

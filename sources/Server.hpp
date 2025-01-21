#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <unistd.h>
#include "Socket.hpp"
#include "UtilParsing.hpp"


typedef struct s_DefaultParamsServer
{
    std::map<std::string, std::vector<std::string> > params;
    s_DefaultParamsServer() 
    {
        params["server_name"].push_back("localhost;");
        params["listen"].push_back("8080;");
        params["client_max_body_size"].push_back("200M;");
        params["upload_path"].push_back("./upload;");
    }
}t_DefaultParamsServer;


class Server
{
    private:
        std::vector<std::string> _basicData;
        std::vector<std::string> _defaultConfServer;
        std::map<int, std::vector<std::string> > _vectRoads;
        std::map<std::string, std::string> _mapConfDefault;

    public:
        void initDefaultConfServ();
        Server(std::vector<std::string> data);
        std::vector<std::string> addValuesRoads(std::vector<std::string>::iterator &cursor);
        void createMapDefaultConf();
        ~Server();

};

#endif

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
    void addValuesParamsServerDefault(std::map<std::string, std::vector<std::string> > mapRecover);
    void printStructMapDefault();
}   t_DefaultParamsServer;




class Server
{
    private:
        std::vector<std::string> _basicData; // donnee donner par le cluster sur toutes les infos du server
        std::vector<std::string> _defaultConfServer; // vector avec infos par default du serveur
        std::map<int, std::vector<std::string> > _vectRoads; // map avec les different localisation du server
        std::map<std::string, std::string> _mapConfDefault; // map ranger des valeurs default key + value
        std::map<int, std::map<std::string, std::vector<std::string> > > _allMapRoads; // map avec toutes les info location avec un index
        int _numberRoads; // commence bien a 0

    public:
        void initDefaultConfServ(); // ou tout ce lance + seraparation default et diffente road
        Server(std::vector<std::string> data); // constructor
        std::vector<std::string> addValuesRoads(std::vector<std::string>::iterator &cursor); // creer vector avec les 'localisation' differentes roads du server
        void createMapDefaultConf(); // map avec default conf des serveurs
        void createMapRoads(int nb); // map avec toutes les infos roads
        ~Server();

};

#endif

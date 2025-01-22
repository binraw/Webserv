#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>

#include <netdb.h>
#include <sys/socket.h>

#include "UtilParsing.hpp"
#include "/home/fberthou/42common_core/webrob/includes/webserv.hpp"


typedef struct s_paramsServer
{
    std::map<std::string, std::vector<std::string> > params;
    s_paramsServer() 
    {
        params["server_name"].push_back("localhost");
        params["listen"].push_back(DFLT_LISTENPORT);
        params["listen"].push_back("8001");
        params["listen"].push_back("8002");
        params["client_max_body_size"].push_back("200M");
        params["upload_path"].push_back("./upload");
    }
} t_paramsServer;

#include <iostream>

class Server
{
	public:
		Server(/* const std::vector<std::string> & data */);
		Server(const Server &);
		~Server();
		Server & operator=(const Server &);

		std::vector<std::string>	addValuesRoads(std::vector<std::string>::iterator &cursor); // creer vector avec les 'localisation' differentes roads du server
        void						initDefaultConfServ(); // ou tout ce lance + seraparation default et diffente road
        void						createMapDefaultConf(); // map avec default conf des serveurs

    private:
        std::vector<std::string>					_basicData; // donnee donner par le cluster sur toutes les infos du server
        std::vector<std::string>					_defaultConfServer; // vector avec infos par default du serveur
        std::map<int, std::vector<std::string> >	_vectRoads; // map avec les different localisation du server
        std::map<std::string, std::string>			_mapConfDefault; // map ranger des valeurs default key + value

        protoent        *_protoName;
        std::set<int>   _ipv4FdSet;
        t_paramsServer  _params;
};

std::ostream	& operator<<(std::ostream &, const Server &);

#endif

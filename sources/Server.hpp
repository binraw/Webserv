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
#include "../includes/webserv.hpp"


typedef struct s_paramsServer
{
    std::map<std::string, std::set<std::string> > params;
    s_paramsServer() 
    {
        params["server_name"].insert("localhost");
        params["client_max_body_size"].insert("200M");
        params["upload_path"].insert("./upload");
    }
} t_paramServer;

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

        t_paramServer &             getParams() const;
    
    private:
        std::vector<std::string>					_basicData; // donnee donner par le cluster sur toutes les infos du server
        std::vector<std::string>					_defaultConfServer; // vector avec infos par default du serveur
        std::map<int, std::vector<std::string> >	_vectRoads; // map avec les different localisation du server
        std::map<std::string, std::string>			_mapConfDefault; // map ranger des valeurs default key + value

        protoent        *_protoName;
        
        void            setParams();
        t_paramServer  _params;

        std::set<int>   _ipv4FdSet;
};

std::ostream	& operator<<(std::ostream &, const Server &);
std::ostream	& operator<<(std::ostream &, const t_paramServer &);

#endif

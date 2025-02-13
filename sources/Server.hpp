#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "ServerConfig.hpp"
# include "Client.hpp"

class Server
{
	public:
		Server(ServerConfig &, const std::string&);
		Server(const Server &);
		~Server();
		Server	&operator=(const Server &);
		bool 	operator<(const Server &other) const;

		const ServerConfig	&getConfig() const;
		const std::string	&getService() const;
		const std::set<std::string>	&getNameList() const;
		const std::map<int, Client>	&getClientList() const;
		const std::set<std::string>	&getLocationPath() const;

	private:
		ServerConfig	_config;//contient un index pour avoir l'ordre de creation des serveurs virtuel suivant le fichier de config
								//cette index est la uniquement pour pouvoir maper les server comme une key dans std::map
		
		std::string				_service;		//service name(port)
		std::set<std::string>	_nameList;		//localhost - serverExemple.com - www.serverExemple.fr
		std::set<std::string>	_locationPath;	//websites handle by the server (/website/site1.com)
		std::map<int, Client>	_clientList;
		// std::set<Location>	_locations
		
		std::map<int, Client>::iterator	findClient(const int);
		void	setLocationPath();

};
std::ostream	& operator<<(std::ostream &, const Server &);

#endif

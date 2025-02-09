#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "ServerConfig.hpp"
# include "Client.hpp"

class Server
{
	public:
		Server(const std::vector<std::string> & data, Cluster &);
		Server(const Server &);
		~Server();
		Server	&operator=(const Server &);
		bool 	operator<(const Server &other) const;

		const ServerConfig		&getConfig() const;
		const std::map<int, Client>	&getClientList() const;
		const std::set<std::string>	&getNameList() const;
		const std::set<std::string>	&getServiceList() const;
		const std::set<std::string>	&getLocationPath() const;

	private:
		const ServerConfig	_config;
		
		std::set<std::string>	_nameList;		//localhost - serverExemple.com - www.serverExemple.fr
		std::set<std::string>	_serviceList;	//port and service name
		std::set<std::string>	_locationPath;	//websites handle by the server (/website/site1.com)
		std::map<int, Client>	_clientList;
		// std::set<Location>	_locations

		void	setLocationPath();

};
std::ostream	& operator<<(std::ostream &, const Server &);

#endif

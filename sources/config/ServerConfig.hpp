


#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "../includes/webserv.hpp"
# include "LocationConfig.hpp"

class ServerConfig 
{
public:
	ServerConfig() {};
	ServerConfig(const ServerConfig &);
	std::vector<std::string>	_serverName;
	std::vector<std::string>	_listenPort;
	std::vector<LocationConfig>	_locationConfig;
	
	std::string	_clientMaxBodySize;
	std::string	_uploadPath;

	void displayValueServer();
	void displayAllLocations();
	void controlDefaultServerConf();
	void checkSemiColonServer();
};
std::ostream & operator<<(std::ostream & o, const ServerConfig &);
#endif

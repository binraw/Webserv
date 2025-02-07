#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP


#include "../includes/webserv.hpp"
#include "ServerConfig.hpp"

class HttpConfig 
{
	public:
		HttpConfig(){}
		HttpConfig(const HttpConfig &);
		std::string	_default_type;
		std::string	_keepalive_timeout;	// should be an integer
		std::string	_worker_connexion;	// should be an integer

		std::set<std::string>		_serviceList;
		std::vector<std::string>	_include;

		std::vector<ServerConfig>	_servers;

		void	displayDefaultHttp();
		void	displayServers();
		void	controlDefaultHttpConf();
		void	checkSemiColonAllValues();

		std::string	getDefaultType();
		std::string	getKeepalive();
		std::string	getWorkerConnexion();

		std::vector<std::string>	getIncludes();
};

std::ostream & operator<<(std::ostream & o, const HttpConfig &);

#endif




#include "HttpConfig.hpp"
#include "UtilParsing.hpp"



HttpConfig::HttpConfig(const HttpConfig & ref)
  : _default_type(ref._default_type), _keepalive_timeout(ref._keepalive_timeout), \
	_worker_connexion(ref._worker_connexion), _include(ref._include), \
	_servers(ref._servers), _serviceList(ref._serviceList)
{   }

std::vector<std::string> HttpConfig::getIncludes() {
	return (_include);
}

std::string HttpConfig::getDefaultType() {
	return (_default_type);
}

std::string HttpConfig::getKeepalive() {
	return (_keepalive_timeout);
}

std::string HttpConfig::getWorkerConnexion() {
	return (_worker_connexion);
}

void HttpConfig::displayDefaultHttp()
{
	for (std::vector<std::string>::iterator it = _include.begin(); it != _include.end(); it++)
		std::cout << *it << " " << std::endl;
	std::cout <<  getDefaultType() << std::endl;
	std::cout <<  getKeepalive() << std::endl;
	std::cout << getWorkerConnexion() << std::endl; 
}

void HttpConfig::displayServers() {
	for(std::vector<ServerConfig>::iterator it = _servers.begin(); it != _servers.end(); it++)
		it->displayValueServer();
}

void HttpConfig::controlDefaultHttpConf()
{
	if (_include.empty())
		throw std::invalid_argument("'Include' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
	if (_default_type.empty())
		throw std::invalid_argument("'default_type' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
	if (_keepalive_timeout.empty())
		throw std::invalid_argument("'keepalive_timeout' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
	if (_worker_connexion.empty())
		throw std::invalid_argument("'worker_connexion' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");

	for (size_t i = 0; i <= _servers.size() - 1; i++)
	{
		_servers[i].controlDefaultServerConf();
		for (std::vector<LocationConfig>::iterator it = _servers[i]._locations.begin(); it != _servers[i]._locations.end(); it++)
			it->controlDefaultLocationConf();
	}
}

void HttpConfig::checkSemiColonAllValues()
{
	if (_default_type.find(";") != std::string::npos)
		_default_type = UtilParsing::trimSemicolon(_default_type);
	if (_keepalive_timeout.find(";") != std::string::npos)
		_keepalive_timeout = UtilParsing::trimSemicolon(_keepalive_timeout);
	if (_worker_connexion.find(";") != std::string::npos)
		_worker_connexion = UtilParsing::trimSemicolon(_worker_connexion);
	for (std::vector<std::string>::iterator it = _include.begin(); it != _include.end(); it++)
	{
		if (it->find(";") != std::string::npos)
			*it = UtilParsing::trimSemicolon(*it);
	}
	for (size_t i = 0; i <= _servers.size() - 1; i++)
	{
		_servers[i].checkSemiColonServer();
		for (std::vector<LocationConfig>::iterator y = _servers[i]._locations.begin(); y != _servers[i]._locations.end(); y++)
			y->checkSemiColonLocation();
	}
}

std::ostream & operator<<(std::ostream & o, const HttpConfig &ref)
{
	o   << "_default_type: " << ref._default_type << std::endl
		<< "_keepalive_timeout: " << ref._keepalive_timeout << std::endl
		<< "_worker_connexion: " << ref._worker_connexion << std::endl
		<< "_includes: ";
	for (std::vector<std::string>::const_iterator it = ref._include.begin();
		it != ref._include.end(); it++)
		o   << *it << " ";

	o   << std::endl << "_serviceList: ";
	for (std::set<std::string>::const_iterator it = ref._serviceList.begin();
		it != ref._serviceList.end(); it++)
		o   << *it << " ";

	o   << std::endl << "_servers:\n" ;
	for (std::vector<ServerConfig>::const_iterator it = ref._servers.begin();
		it != ref._servers.end(); it++)
		o << *it << std::endl;
	return o;
}
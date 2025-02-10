


/*============================================================================*/
							/*### HEADER FILES ###*/
/*============================================================================*/
#include "webserv.hpp"
#include "Server.hpp"
#include "UtilParsing.hpp"

/*============================================================================*/
			/*### CONSTRUCTORS - DESTRUCTOR _ OVERLOAD OPERATORS ###*/
/*============================================================================*/

Server::Server(const ServerConfig & config)
  : _config(config)
{
	UtilParsing::convertVectorToSet(_nameList, _config._serverName);
	UtilParsing::convertVectorToSet(_serviceList, _config._listenPort);
	setLocationPath();
	_clientList.clear();
}
/*----------------------------------------------------------------------------*/

Server::Server(const Server &ref)
  : _config(ref._config)
{
	UtilParsing::deepCopieSet(_nameList, ref._nameList);
	UtilParsing::deepCopieSet(_locationPath, ref._locationPath);
	_serviceList = ref._serviceList;
	_clientList = ref._clientList;
}
/*----------------------------------------------------------------------------*/

Server::~Server() 
{	}
/*----------------------------------------------------------------------------*/

Server  & Server::operator=(const Server &ref)
{
	UtilParsing::deepCopieSet(_nameList, ref._nameList);
	UtilParsing::deepCopieSet(_serviceList, ref._serviceList);
	UtilParsing::deepCopieSet(_locationPath, ref._locationPath);

	return *this;
}
/*----------------------------------------------------------------------------*/

bool	Server::operator<(const Server &ref) const {
	return _serviceList < ref._serviceList;
}
/*----------------------------------------------------------------------------*/

std::ostream & operator<<(std::ostream & o, const Server &ref)
{
	o	<< BOLD "SERVER:" RESET << std::endl
		<< "_nameList: ";
	for (std::set<std::string>::const_iterator it = ref.getNameList().begin();
		it != ref.getNameList().end(); it++)
			o	<< "[" << *it << "] ";
	
	o	<< "\n_serviceList: ";
	for (std::set<std::string>::const_iterator it = ref.getServiceList().begin();
		it != ref.getServiceList().end(); it++)
			o	<< "[" << *it << "] ";
	
	o	<< "\n_locationPath: ";
	for (std::set<std::string>::const_iterator it = ref.getLocationPath().begin();
		it != ref.getLocationPath().end(); it++)
			o	<< "[" << *it << "] ";

	// o	<< "\n_clientList: ";
	// for (std::set<Client>::const_iterator it = ref.getClientList().begin();
	// 	it != ref.getClientList().end(); it++)
	// 		o	<< *it ;
	return o << RESET << std::endl;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PUBLIC METHODS ###*/
/*============================================================================*/

const std::set<std::string>	& Server::getNameList() const {
	return const_cast<std::set<std::string> &>(_nameList);
}
/*----------------------------------------------------------------------------*/

const std::set<std::string> & Server::getServiceList() const {
	return const_cast<std::set<std::string>&>(_serviceList);
}
/*----------------------------------------------------------------------------*/

const std::set<std::string>	& Server::getLocationPath() const {
	return const_cast<std::set<std::string>&>(_locationPath);
}
/*----------------------------------------------------------------------------*/

const std::map<int, Client>	&Server::getClientList() const {
	return _clientList;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PRIVATE METHODS ###*/
/*============================================================================*/
void	Server::setLocationPath()
{
	for (std::vector<LocationConfig>::const_iterator it = _config._locationConfig.begin();
		it != _config._locationConfig.end(); it++) {
			_locationPath.insert(it->_path);
		}
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
							/*### EXCEPTIONS ###*/
/*============================================================================*/




/*============================================================================*/
							/*### HEADER FILES ###*/
/*============================================================================*/
#include "../includes/webserv.hpp"
#include "Cluster.hpp"

/*============================================================================*/
			/*### CONSTRUCTORS - DESTRUCTOR _ OVERLOAD OPERATORS ###*/
/*============================================================================*/

Server::Server(const std::vector<std::string> &, Cluster & )
  :	_backLog(1024)
{
	UtilParsing::convertVectorToSet(_nameList, _config._serverName);
	UtilParsing::convertVectorToSet(_serviceList, _config._listenPort);
	setLocationPath();
	_clientList.clear();
}
/*----------------------------------------------------------------------------*/

Server::Server(const Server & ref)
  : _backLog(ref._backLog)
{	}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        /*### DESTRUCTORS ###*/
/*============================================================================*/

Server::~Server() 
{	}
/*----------------------------------------------------------------------------*/

<<<<<<< HEAD
/*============================================================================*/
                    /*### OVERLOAD OPERATOR ###*/
/*============================================================================*/
Server  & Server::operator=(const Server &)
{ return *this; }
/*----------------------------------------------------------------------------*/

std::ostream	& operator<<(std::ostream & o, const Server & ref)
{
	o	<< BOLD BRIGHT_CYAN "SERVER:\n" << std::endl
		<< BOLD BRIGHT_CYAN "_backlog: " RESET CYAN << ref.getBacklog()
		<< std::endl;
=======
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
>>>>>>> 8db5554 (intergration of the parsing terminated. Analysis of GET and POST requests. clean compilation)

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

const int	& Server::getBacklog() const {
	return _backLog;
}
/*----------------------------------------------------------------------------*/

const std::set<std::string>	& Server::getNameList() const {
	return const_cast<std::set<std::string> &>(_nameList);
}
/*----------------------------------------------------------------------------*/

const std::set<std::string> & Server::getServiceList() const {
	return const_cast<std::set<std::string>&>(_serviceList);
}
/*----------------------------------------------------------------------------*/

const std::set<std::string>	& Server::getNameList() const {
	return const_cast<std::set<std::string>&>(_nameList);
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

<<<<<<< HEAD
/*============================================================================*/
                        /*### PUBLIC METHODS ###*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/

=======
>>>>>>> 8db5554 (intergration of the parsing terminated. Analysis of GET and POST requests. clean compilation)
/*============================================================================*/
							/*### EXCEPTIONS ###*/
/*============================================================================*/

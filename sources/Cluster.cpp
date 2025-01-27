



/*============================================================================*/
						/*### HEADER FILES ###*/
/*============================================================================*/
#include "Cluster.hpp"

/*============================================================================*/
					/*### CONSTRUCTORS (DEFAULT & COPY) ###*/
/*============================================================================*/

Cluster::Cluster(const std::string &filename) throw(std::exception)
{
	_configPath = filename;
	std::ifstream inputFile(filename.c_str());
	std::string line;
	if (!inputFile.is_open()) 
	{
		std::cerr << "Could not open conf file" << std::endl;
		return;
	}
	while (std::getline(inputFile, line))
	{
		if (line.empty() || line[0] == '#') continue;
		_allConf = UtilParsing::split(line, std::string(" "));
	}
	inputFile.close();
	initDefaultConf();
	
	setAllServer();
}
/*----------------------------------------------------------------------------*/

Cluster::Cluster(const Cluster & )
{
	std::cout << "HHHHHHHHHHHHHHHHERE" ;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### DESTRUCTORS ###*/
/*============================================================================*/
Cluster::~Cluster()
{   }
/*----------------------------------------------------------------------------*/

/*============================================================================*/
					/*### OVERLOAD OPERATOR ###*/
/*============================================================================*/
Cluster &	Cluster::operator=(const Cluster & ) {
	return *this;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        	/*### GETTER ###*/
/*============================================================================*/
const std::string	& Cluster::getFileConfig() const {
	return const_cast<std::string &>(_configPath);
}
/*----------------------------------------------------------------------------*/

const t_paramsServer	& Cluster::getParams()	 const {
	return const_cast<t_paramsServer &>(_params);
}
/*----------------------------------------------------------------------------*/

const std::vector<Server>	& Cluster::getAllServer() const{
	return const_cast<std::vector<Server> & >(_servers);
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        	/*### SETTERS ###*/
/*============================================================================*/

void	Cluster::setParams()
{	/*code*/	}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PRIVATE METHODS ###*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PUBLIC METHODS ###*/
/*============================================================================*/
// Ici je recolte les servers que je mets dans une map 
void Cluster::initDefaultConf()
{
	std::vector<std::string>::iterator it;
	int i = 0;
	int number_servers = 0;
	for(it = _allConf.begin(); it != _allConf.end(); it++)
	{
		if (*it == std::string("{"))
			i++;
		if (*it == std::string("server"))
		{
			std::pair<int, std::vector<std::string> > serverPair(number_servers, addValuesServers(it));
			_vectServers.insert(serverPair);
			number_servers++;
		}
		if (*it == std::string("}"))
			i--;
		if (i <= 1)
			_defaultConf.push_back(*it);
	}
	cleanClusterConfDefault();
}
/*----------------------------------------------------------------------------*/

std::vector<std::string> Cluster::addValuesServers(std::vector<std::string>::iterator &cursor)
{
	std::vector<std::string> server;
	std::vector<std::string>::iterator it;
	server.push_back(*cursor);
	int i = 0;
	int y = 0;
	for(it = cursor;it != _allConf.end(); it++)
	{
		if (*it == std::string("{"))
			i++;
		if (*it == std::string("}"))
			i--;
		if (i > 0)
			server.push_back(*it);
		if (i == 0 && y > 0)
			break;
		y++;
	}
	// std::cout << "Value du serveur :" << std::endl;
	// for (size_t y = 0; y < server.size(); y++) 
	// {
	//	 std::cout << server[y] << std::endl;
	// }
	return server;
}
/*----------------------------------------------------------------------------*/

void Cluster::setAllServer()
{
	for(std::map<int, std::vector<std::string> >::const_iterator it = _vectServers.begin(); \
		it != _vectServers.end(); it++)	{
		_servers.push_back(Server(it->second));
	}
}
/*----------------------------------------------------------------------------*/

void Cluster::cleanClusterConfDefault()
{
	_defaultConf = UtilParsing::cleanVector(_defaultConf);
	for (std::vector<std::string>::iterator it = _defaultConf.begin(); it != _defaultConf.end(); )
	{
		if (*it == "{" || *it == "}" || *it == "http" || *it == "server") 
		{ 
			it = _defaultConf.erase(it);
		} else 
			++it;
	}
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
							/*### EXCEPTIONS ###*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/

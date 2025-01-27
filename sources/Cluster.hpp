#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "Server.hpp"

#include <sstream>
#include <fstream>


typedef struct s_paramsServer
{
	std::map<std::string, std::string>
		mimes;
	std::map<std::string, std::vector<std::string> >
		params;
	s_paramsServer() 
	{
		mimes[".bin"].assign("application/octet-stream");
		mimes[".js"].assign("application/javascript");
		mimes[".doc"].assign("application/msword");
		mimes[".csh"].assign("application/x-csh");
		mimes[".json"].assign("application/json");
		mimes[".pdf"].assign("application/pdf");
		mimes[".sh"].assign("application/x-sh");
		mimes[".jpeg"].assign("image/jpeg");
		mimes[".jpg"].assign("image/jpeg");
		mimes[".bmp"].assign("image/bmp");
		mimes[".gif"].assign("image/gif");
		mimes[".png"].assign("image/png");
		mimes[".html"].assign("text/html");
		mimes[".htm"].assign("text/html");
		mimes[".css"].assign("text/css");
		mimes[".csv"].assign("text/csv");

		params["includes"].push_back("./error_pages");
		params["default_type"].push_back("application/octet-stream;");
		params["keepalive_timeout"].push_back("65;");
		params["worker_connexion"].push_back("1024;");
	}

}	t_paramsCluster;

class Cluster
{
	public:
	/*	* CANONICAL FORM
		*
	*/
		Cluster(const std::string &filename) throw(std::exception);
		~Cluster();

	/*	* PUBLIC METHODS
		*
	*/
		void	initDefaultConf();			// init la default conf et separe les serveurs
		void	cleanClusterConfDefault();	// clean le vector default conf
		std::vector<std::string>
				addValuesServers(std::vector<std::string>::iterator &cursor);

	/*	* GETTERS
		*
	*/
		const std::string			& getFileConfig()	const;
		const t_paramsCluster		& getParams()		const;
		const std::vector<Server>	& getAllServer()	const;
		const std::set<int>			& getSockFds()		const;

	protected:
		void	addSocket();
	private:
	/*	* PRIVATE CONSTRUCTORS
		*
	*/
	Cluster(const Cluster &);
	Cluster	& operator=(const Cluster &);

	/*	* PRIVATE MEMBERS
		*
	*/
		std::string			_configPath;	// chemin vers fichier de config
		t_paramsCluster		_params;		// config par defaut
		std::vector<Server>	_servers;		// ensemble des servers present dans le cluster
		std::set<int>		_sockFds;

	/*	* SETTERS
		*
	*/
		void	setParams();	// init le membre prive _params
		void	setAllServer();	// init le membre prive _servers 
	
	/*	* PRIVATE METHODS
		* 1er bloc parsing
		* 2em bloc Init
	*/
		std::vector<std::string>	_allConf;		// tout le fichier
		std::vector<std::string>	_defaultConf;	// partie default du cluster clean
		std::map<int, std::vector<std::string> >
				_vectServers;					 	// utilitaire sous forme de map pour activer les servers
		std::map<std::string, std::string>
				_mapDefaultParamsCluster;			// futur map default conf

	/*	* EXCEPTION
		*
	*/

};
/*	* OSTREAM OPERATOR
	* 
*/
std::ostream	& operator<<(std::ostream &, Cluster &);
std::ostream	& operator<<(std::ostream &, const t_paramsCluster &);

#endif

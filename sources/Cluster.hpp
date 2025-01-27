#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "UtilParsing.hpp"
#include "Server.hpp"
#include <vector>

#include <sstream>
#include <fstream>
#include <string>
#include <cstring>


typedef struct s_DefaultParams
{
	std::map<std::string, std::string>
		mimes;
	std::map<std::string, std::vector<std::string> >
		params;
	s_DefaultParams() 
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
}	t_DefaultParams;

class Cluster
{
	public:
		Cluster(const std::string &filename);	// constructor
		Cluster(const Cluster &);				// copy
		~Cluster();
		Cluster &	operator=(const Cluster &);

		void
			initDefaultConf(); // init la default conf et separe les serveurs
		std::vector<std::string>
			addValuesServers(std::vector<std::string>::iterator &cursor);
		void
			cleanClusterConfDefault(); // clean le vector default conf
		void
			createMapDefaultConf(); // ici la fonction que jessaye de faire pour la map avec key + value de la defaultconf
		void
			initAllServer(); // init tout les serveurs et les crees 
	
	protected:
		

	private:
		std::string
				_configPath;		// chemin du fichier de conf
		std::vector<std::string>
				_allConf;			// tout le fichier
		std::vector<std::string>
				_defaultConf;		// partie default du cluster clean
		
		std::map<int, std::vector<std::string> >
				_vectServers; 				// utilitaire sous forme de map pour activer les servers
		std::map<std::string, std::string>
				_mapDefaultParamsCluster;	// futur map default conf
		
		t_DefaultParams
				defaultParams;				// default conf (createur)

		std::vector<Server>
				_servers;					// ensemble des servers present dans le cluster
};

#endif

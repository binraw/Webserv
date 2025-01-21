


#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "webserv.hpp"

#include <map>
#include <string>
#include <vector>

#include <netdb.h>

typedef struct s_clusterDefault
{
	std::map<std::string, std::vecor<std::string> >
		defaultParams;
		
	s_clusterDefault()
	{
		defaultParams =
		{
			{ "includes", { "./includes/mime.types" } },
			{ "default_type", { "application/octet-stream" } },
			{ "listen", { "8000" } }
			// a completer au fil du developpement
		};
	}
}	t_clusterDefault;

class Cluster
{
	public:
		Cluster(const std::string & filePath);
		Cluster(const Cluster &);
		~Cluster();
		
		Cluster &				operator=(const Cluster &);
		friend std::ostream &	operator<<(std::ostream &, const Cluster &);

	private:
		// MEMBERS //
		const std::string	& _filePath;
		// copie du path du fichier de config pour eviter de le passer en parametre aux fonctions

		t_clusterDefault	_clusterDefault;
		// structure qui contient tous les parametres du cluster par defaut

		protoent	* protoentry;
		// structure initialisee par la fonction getprotobyname("tcp");

		std::map<int, std::vector<Server> >	servers;
		/*  _service_servers
			structure finale qui contient tous les serveurs
			schema de la structure attendue
			_service_servers {
				{1, {AServer1*, AServer2*, AServer3*}},
				{2, {AServer1*, AServer2*, AServer3*}}
			}
		*/

		// METHODS //
};

#endif
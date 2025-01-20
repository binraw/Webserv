


#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "webserv.hpp"

#include <map>
#include <string>
#include <vector>

#include <netdb.h>

class AServer;
class ClusterException;

/*
	structure des parametres par default de l'ensemble des clusters
*/
struct s_clusterDefault
{
	std::vector<std::string>	protocols_handle_by_webserv;
	std::vector<int>			nb_virtual_server;
		
	s_clusterDefault()
	  : protocols_handle_by_webserv(1, "http"),
		nb_virtual_server(1, 1)
	{   }
};

/*
	contient les 
*/
struct s_protocolData {
	protoent					* protocolInfo;
	std::vector<std::string>	include;
	std::string					default_type;
	int							keepalive_timeout;
	int							worker_connexion;

	s_protocolData()
	  : protocolInfo(getprotobyname("tcp")),
	  	include( { PATH_MIME } ),
		default_type( { "application/octet-stream" } ),
		keepalive_timeout( DFLT_TIMEOUT ),
		worker_connexion( DFLT_WORKCONNEX )
	{   }
};

typedef struct s_clusterDefault t_clusterDefault;
typedef struct s_protocolData   t_protocolData;

class Cluster
{
	public:
		Cluster(const std::string & file) throw(ClusterException);
		Cluster(const Cluster &);
		virtual ~Cluster();
		
		Cluster &				operator=(const Cluster &) const;
		friend std::ostream &	operator<<(std::ostream &, const Cluster &);
	protected:


	private:
		// MEMBERS //
		const std::string
			& _configPath;
		/*  _configPath
			copie du path du fichier de config pour eviter de le passer en parametre aux fonctions
		*/

		t_clusterDefault
			_clusterDefault;
		/*  _clusterDefault
			structure qui contient tous les parametres du cluster par defaut
			elle permetra de pouvoir lancer un cluster classique http
			ou bien plusieur cluster avec des protocoles differents
		*/

		std::map<std::string, std::vector< std::string> >
			& _configData;
		/*  _configData
			cette map est remplie dans la fonction "parse file"
			key : nom du protocole
			val : vector qui contient tous les parametres des serveurs

			cette structure de donnee est le resultat final du parsing des blocs protocole (http / https)
			elle sera donnee en argument a la fonction d'initialisation des serveurs
		*/

		std::map<std::string, std::vector<AServer *> >
			_service_servers;
		/*  _service_servers

			schema de la structure attendue
			_service_servers {
				{"http", {AServer1*, AServer2*, AServer3*}},
				{"https", {AServer1*, AServer2*, AServer3*}}
			}
		*/

		// METHODS //
		std::map<std::string, std::map<std::string, std::string>>
			& parseFile(void);

		std::vector<std::string>
			& getFile(void);

		std::map<std::string, std::map<std::string, std::string>>
			& getAllProto(const std::vector<std::string> & allFile);

		void	
			setCluster(const std::vector<std::string> &, \
						std::map<std::string, std::map<std::string, std::string>> &);
		void
			majClusterDefault(const std::vector<std::string> &);
		/*
			infos necessaires avant initialisation des serveurs :
				- le nb de protocoles differents (HTTP HTTPS IRC etc)
				- le nb de serveur virtuelles a initialiser par protocoles

			il me faut une liste de parametres par protocole
				pour modifier les parametres static du tableau de params de Cluster
				un tableau de params par protocoles (si plusieurs sont pris en charge, renvoie une erreur si le protocole n'est pas pris en charge (ex HTTPS))

			
		*/

		// EXCEPT CLASS //
		class ClusterException : std::exception
		{
			public:
				ClusterException() {};
				void			openfile(const std::string & filepath) const throw();
				const char *	what() const throw();

		};
};

#endif
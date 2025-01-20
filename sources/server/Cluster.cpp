

#include <iostream>

/*============================================================================*/
						/*### HEADER FILES ###*/
/*============================================================================*/

#include "Cluster.hpp"
#include "UtilParsing.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


/*============================================================================*/
					/*### INITIALISATION STATIC FIELD ###*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/

/*============================================================================*/
					/*### CONSTRUCTORS (DEFAULT & COPY) ###*/
/*============================================================================*/

Cluster::Cluster(const std::string & file)
  : _configPath(file),
	_configData(parseFile())
{
	/*
		a partir de ce moment, _clusterParams doit etre a jour

		_service_server doit etre prepare pour opti la memoire
		std::map<std::string, std::vector<AServer*>> _service_server
	*/
   _service_servers = initService(_clusterData);
}
/*----------------------------------------------------------------------------*/

Cluster::Cluster(const Cluster & ref)
{   }
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

Cluster & Cluster::operator=(const Cluster & ref) const
{   }
/*----------------------------------------------------------------------------*/

std::ostream & operator<<(std::ostream & o, const Cluster & right)
{   }
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### GETTER - SETTER ###*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PRIVATE METHODS ###*/
/*============================================================================*/

std::map<std::string, std::map<std::string, std::string>>
		& Cluster::parseFile(void)
{
	const   std::vector<std::string>
			allFile = getFile();
#ifdef TEST
	std::cout   << "FILE AFTER SPLIT METHOD: " << std::endl;
	for (size_t i = 0; i < allFile.size(); i++) {
		std::cout   << allFile[i] << std::endl;
	}
#endif
	return getAllProto(allFile);
}
/*----------------------------------------------------------------------------*/

std::vector<std::string>
		& Cluster::getFile(void) // throw (ClusterException)
{
	char		buffer[4096] = { '\0' };
	const int   fd = open(_configPath.c_str(), O_RDONLY | O_CLOEXEC);
	std::string allFile;

	if (fd < 0)
		throw ClusterException().openfile(_configPath);
		
	for (ssize_t i = read(fd, buffer, sizeof(buffer)); i != 0;)
	{
		if (i == -1)
			throw ClusterException().openfile(_configPath);
		allFile += buffer;
		memset(buffer, '\0', sizeof(buffer));
	}

	if (close(fd) == -1)
		throw ClusterException().openfile(_configPath);

	return UtilParsing::split(allFile, std::string(" "));
}
/*----------------------------------------------------------------------------*/

std::map<std::string, std::map<std::string, std::string>>
		& Cluster::getAllProto(const std::vector<std::string> & allFile)
{
	/*
		l'argument <allFile> contient TOUT le fichier de config, 1 mot par case du tableau
		
		la fonction doit extraire chaque bloc protocole de allFile et construire
		une map<> dont voici un exemple :
		std::map<std::string, std::map<std::string, std::string>> result {
			"http" {
				"server", { "{", "server_name", "localhost", ";", "listen", "8080", ";", "client_max_body_size", "200M", ";", "}" },
				"server", { "{", "server_name", "bidule", ";", "listen", "8081", ";", "}" },
				"server", { "{", "server_name", "127.0.0.1", ";", "listen", "8082", ";", "}" }
			}, 
			"https" {
				"server", { "{", "server_name", "localhostSecure", ";", "listen", "443", ";", "client_max_body_size", "200M", ";", "}" },
				"server", { "{", "server_name", "biduleSecure", ";", "listen", "443", ";", "}" },
				"server", { "{", "server_name", "127.0.0.1", ";", "listen", "443", ";", "}" }
			}
		}

		la fonction doit aussi identifier les information qui sont dans le bloc protocole et en dehors des blocs serveur
		elle doit mettre a jour le tableau static _clusterParams si necessaire AVANT DE RENVOYER LA MAP<>

	*/


	std::map<std::string, std::map<std::string, std::string>>
		result;
	std::vector<std::string>
		clusterParams;
		
	this->setCluster(clusterParams, result);
	return result;
}
/*----------------------------------------------------------------------------*/

void
	Cluster::setCluster(const std::vector<std::string> & clusterParams, \
						std::map<std::string, std::map<std::string, std::string>> & ref)
{
	/*  UTILISER <ALGORITexpressionHM>
		la fonction met a jour la structure _clusterdefault de l'instance courante
		avec les parametres donnes dans clusterParams
		
		affiche une erreur si un cluster demande un protocole non prit en compte
		
		initialise le container _service_servers a la bonne taille
		grace a ref
	*/

	// mettre a jour les infos des clusters
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PUBLIC METHODS ###*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/

/*============================================================================*/
							/*### EXCEPTIONS ###*/
/*============================================================================*/

const char
		* Cluster::ClusterException::what() const throw() {
	return ("During Cluster initilization\n");
}
/*----------------------------------------------------------------------------*/

void
		Cluster::ClusterException::openfile(const std::string & file) const throw()
{
	std::cerr   << RED ERROR
				<< file << " could not be open, check path & authorisation"
				<< RESET << std::endl;
}
/*----------------------------------------------------------------------------*/

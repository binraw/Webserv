#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "Server.hpp"
#include <vector>
#include "UtilParsing.hpp"

typedef struct s_DefaultParams
{
    std::map<std::string, std::vector<std::string> > params; // mettre juste std::string et std::string ?
    // car je ne vois pas utiliter de faire un vector en deuxieme arg
    s_DefaultParams() 
    {
        params["includes"].push_back("./error_pages ./mime.types;");
        params["default_type"].push_back("application/octet-stream;");
        params["keepalive_timeout"].push_back("65;");
        params["worker_connexion"].push_back("1024;");
    }
}t_DefaultParams;


class Cluster
{
private:
    std::string _configPath; // chemin du fichier de conf
    std::vector<std::string> _allConf; // tout le fichier
    std::vector<std::string> _defaultConf; // partie default du cluster clean
    std::map<int, std::vector<std::string> > _vectServers; // utilitaire sous forme de map pour activer les servers
    std::vector<Server> _servers; // ensemble des servers present dans le cluster
    t_DefaultParams defaultParams; // default conf (createur)
    std::map<std::string, std::string> _mapDefaultParamsCluster; // futur map default conf


public:
    Cluster();
    ~Cluster();
    Cluster(const std::string &filename); // constructor
    void initDefaultConf(); // init la default conf et separe les serveurs
    std::vector<std::string> addValuesServers(std::vector<std::string>::iterator &cursor);
    void cleanClusterConfDefault(); // clean le vector default conf
    void createMapDefaultConf(); // ici la fonction que jessaye de faire pour la map avec key + value de la defaultconf
    void initAllServer(); // init tout les serveurs et les crees 
    static std::map<std::string, std::string *> _defaultTab;
};




#endif

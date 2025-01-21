#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "Server.hpp"
#include <vector>
#include "UtilParsing.hpp"

typedef struct s_DefaultParams
{
    std::map<std::string, std::vector<std::string> > params;
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
    std::string _configPath;
    std::vector<std::string> _allConf;
    std::vector<std::string> _defaultConf;
    std::map<int, std::vector<std::string> > _vectServers; // appeler les servers avec ces vectors et tout faire dedans
    std::vector<Server> _servers; // faire push back de chaque construct server
    t_DefaultParams defaultParams;



public:
    Cluster();
    ~Cluster();
    Cluster(const std::string &filename);
    void initDefaultConf();
    std::vector<std::string> addValuesServers(std::vector<std::string>::iterator &cursor);
    void displayServersConfig();
    void initAllServer();
    static std::map<std::string, std::string *> _defaultTab;
};




#endif

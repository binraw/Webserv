#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "Server.hpp"
#include <vector>
#include "UtilParsing.hpp"

class Cluster
{
private:
    std::string _configPath;
    std::map<std::string, std::vector<std::string> > _serverParams;
    std::vector<std::string> _allConf;
    std::vector<std::string> _defaultConf;
    std::map<int, std::vector<std::string> > _vectServers;
public:
    Cluster();
    ~Cluster();
    Cluster(const std::string &filename);
    void initDefaultConf();
    std::vector<std::string> initServer(std::vector<std::string>::iterator &cursor);
    void displayServersConfig();
    void initAllServer();
    static std::map<std::string, std::string *> _defaultTab;
};




#endif

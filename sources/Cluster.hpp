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
public:
    Cluster();
    ~Cluster();
    Cluster(const std::string &filename);
    void displayServersConfig();
    void initAllServer();
    static std::map<std::string, std::string *> _defaultTab;
};




#endif

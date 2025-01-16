#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "Server.hpp"
#include <vector>

class Cluster
{
private:
    std::vector<Server> _servers;
public:
    Cluster();
    ~Cluster();
    Cluster(const std::string &filename);
    void initAllServer();
};




#endif

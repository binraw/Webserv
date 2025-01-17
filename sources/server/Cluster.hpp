


#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "webserv.hpp"

#include <string>
#include <map>

#include <netdb.h>

class Server;
class ClusterException;

class Cluster
{
    public:
        Cluster(const std::string & file) throw(ClusterException);
        Cluster(const Cluster &);
        virtual ~Cluster();
        
        Cluster &               operator=(const Cluster &);
        friend std::ostream &   operator<<(std::ostream &, Cluster &);
    
    private:
        // MEMBERS //
        protoent                            * _protocolInfo;
        std::map<std::string, std::string>  & _initInfo;
        // un container map avec un serveur et son socket associe
        std::map<Server, int>               & _cluster;

        // METHODS //
        protoent    * setProtcole(const std::string & proto) throw(ClusterException);
        std::map<std::string, std::string> \
                    & setInitInfo(const std::string & file) throw(ClusterException);
        std::map<Server, int>
                    & setVirtualServer();

        // EXCEPT CLASS //
        class ClusterException : std::exception
        {
            public:
                const char *    what() const throw();
                void            display(std::string & msg) const throw();

        }


} ;

#endif
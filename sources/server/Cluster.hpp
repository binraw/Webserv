


// #ifndef CLUSTER_HPP
// # define CLUSTER_HPP

// #include "webserv.hpp"

// #include <string>
// #include <map>
// #include <vector>

// #include <netdb.h>

// class Server;
// class ClusterException;

// class Cluster
// {
//     public:
//         Cluster(const std::string & file) throw(ClusterException);
//         Cluster(const Cluster &);
//         virtual ~Cluster();
        
//         Cluster &               operator=(const Cluster &) const;
//         friend std::ostream &   operator<<(std::ostream &, const Cluster &);
    
//     private:
//         // MEMBERS //
//         protoent                    * _protocolInfo;    // ok
//         std::vector<std::string>    & _initInfo;        // 
//         std::vector<Cluster>        _cluster;
//         std::vector<Server>         _servers;

//         // METHODS //
//         protoent    * setProtcolInfo(const std::string & proto) throw(ClusterException);
//         std::vector<std::string> \
//                     & setInitInfo(const std::string & file) throw(ClusterException);
//         std::vector<Server>
//                     & setVirtualServer();

//         // EXCEPT CLASS //
//         class ClusterException : std::exception
//         {
//             public:
//                 const char *    what() const throw();
//                 void            display(std::string & msg) const throw();

//         }


// } ;

// #endif
#ifndef SERVER_HPP
# define SERVER_HPP

#include <netdb.h>
#include <string>
#include "AServer.hpp"
#include "AServer.hpp"

struct s_serverDefault
{
	std::vector<std::string>    server_name;
	std::vector<int>            listen;
    int                         clientBodySize;
    int                         keepAliveTimeout;
    std::string                 uploadPath;
		
	s_serverDefault()
	  : server_name(1, {"localhost"}),
		listen(1, 8080),
        clientBodySize(200),
        keepAliveTimeout(65),
        uploadPath(DFLT_UPLOAD)
	{   }
};
	// init d'une map<>
	// std::map<std::string, std::vector<std::string> >
	// 	protocols_handle_by_webserv;
	// protocols_handle_by_webserv({{"http", {"GET", "POST", "PUT"}}})

typedef struct s_serverDefault t_serverDefault;

class Server : virtual public AServer
{
    public:
        Server();
        ~Server();
    
    private:
        protoent    * _protocolInfo;
        protoent    * setProtcolInfo(const std::string & proto)
                    throw(ClusterException); // appele dans constructeur ?


        std::string & _name;
        int         * _sockFd;

};

#endif

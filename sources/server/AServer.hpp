



#ifndef ASERVER_HPP
# define ASERVER_HPP

#include <map>
#include <string>
#include <vector>

#include "webserv.hpp"

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

typedef struct s_serverDefault t_serverDefault;

class AServer
{
    protected:
        AServer(std::map<std::string, std::vector<std::string>> & ref);
        t_serverDefault _defaultParams;

    private:
        virtual ~AServer() {};
};

#endif

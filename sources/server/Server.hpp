#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>

class Server
{
    public:
        Server();
        ~Server();
    
    private:
        std::string & _name;
        int         * _sockFd;

};

#endif

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>

#include "Socket.hpp"


class Server
{
    public:
        Server(const std::string& filename);
        ~Server();
        void init_data();
        std::map<std::string, std::string> parseConf(const std::string& filename);
        void displayConfig();
        int controlMap();
        class ErrorConfigParam: public std::exception
        {
            virtual const char* what() const throw();
        };
    
    private:
        std::map<std::string, std::string> _paramsConfig;
        Socket _socket;

};

#endif

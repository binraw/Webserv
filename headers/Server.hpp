#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <unistd.h>
#include "Socket.hpp"


class Server
{
    private:
        std::map<std::string, std::string> _paramsConfig;
        Socket _socket;

    public:
        Server(const std::map<std::string, std::string> copy_map);
        ~Server();
        void init_data();
        std::map<std::string, std::string> parseConf(const std::string& filename);
        void displayConfig();
        int controlMap();
        class ErrorConfigParam: public std::exception
        {
            virtual const char* what() const throw();
        };
};

#endif

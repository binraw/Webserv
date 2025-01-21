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
#include "UtilParsing.hpp"


class Server
{
    private:
        std::vector<std::string> _basicData;
        std::vector<std::string> _defaultConfServer;

    public:
        void initDefaultConfServ();
        Server(std::vector<std::string> data);
        ~Server();

};

#endif

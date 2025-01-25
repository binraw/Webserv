// #include <iostream>
// #include <sstream>
// #include <string>
// #include <cstring>

// #include <unistd.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <arpa/inet.h>

// #define SERVICE "8080"
// #define BACKLOG 10
// #define BUFFER_SIZE 1024

/*
    * define a port to listen
    * add the local ip address 
    * 
*/





#include "Server.hpp"
#include "Cluster.hpp"

int main(void)
{
    try
    {
        Cluster one("./config/default.conf");
        /* code */
    }
    catch(const std::exception& e)
    {
        std::cout << "dans le main" << std::endl;
        std::cerr << e.what();
    }
    

    // one.initAllServer();
    // one.displayServersConfig();
    return 0;
}

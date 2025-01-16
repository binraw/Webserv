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





#include "../headers/Server.hpp"
#include "../headers/Cluster.hpp"

int main(void)
{
    Cluster one("./config/server.conf");

    one.initAllServer();
    one.displayServersConfig();
    return 0;
}

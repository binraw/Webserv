#include "ConfigParser.hpp"

#include "Cluster.hpp"
#include "Client.hpp"
#include <cstring>
#include <csignal>
#include "CGI.hpp"

void hand(int, siginfo_t *, void *);

int main(void)
{


    try 
    {
        processCGITEST("./cgi-bin/script.pl");
    }
    catch(const std::exception& e) {
        std::cerr	<< YELLOW << e.what() << std::endl
                    << RED "webserv : EXIT_FAILURE"
					<< RESET << std::endl;
        return 1;
    }
    return 0;
}

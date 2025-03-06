#include "ConfigParser.hpp"

#include "Cluster.hpp"
#include "Client.hpp"
#include <cstring>
#include <csignal>
#include "CGI.hpp"

void hand(int, siginfo_t *, void *);
std::string		executeCgi(char		**env);

int main(void)
{


    try 
    {
        char **env = initEnvTEST();
        executeCgi(env);

    }
    catch(const std::exception& e) {
        std::cerr	<< YELLOW << e.what() << std::endl
                    << RED "webserv : EXIT_FAILURE"
					<< RESET << std::endl;
        return 1;
    }
    return 0;
}

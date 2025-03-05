#include "ConfigParser.hpp"

#include "Cluster.hpp"
#include "Client.hpp"
#include <cstring>
#include <csignal>
#include "CGI.hpp"

void hand(int, siginfo_t *, void *);
int decryptHexa(std::string value);

int main(void)
{


    try 
    {
        std::string value = ParseUri("asasa?prenom=Pierre%21&nom=Durand&age=25&ville=Paris");
       std::cout << value << std::endl;
    }
    catch(const std::exception& e) {
        std::cerr	<< YELLOW << e.what() << std::endl
                    << RED "webserv : EXIT_FAILURE"
					<< RESET << std::endl;
        return 1;
    }
    return 0;
}

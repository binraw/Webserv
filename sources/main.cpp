#include "Server.hpp"
#include "Cluster.hpp"
#include "ConfigParser.hpp"

int main(void)
{
    try
    {
        ConfigParser parser;
        HttpConfig config = parser.parse("./config/default.conf");
        config.displayDefaultHttp();
        config.displayServers();
    }
    catch(const std::exception& e)
    {
        std::cout << "dans le main" << std::endl;
        std::cerr << e.what();
    }
    

    return 0;
}

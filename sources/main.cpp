
#include "ConfigParser.hpp"

int main(void)
{
    try
    {
        ConfigParser parser;
        HttpConfig config = parser.parse("./config/default.conf");
        config.controlDefaultHttpConf();
        config.checkSemiColonAllValues();
        config.displayDefaultHttp();
        config.displayServers();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what();
    }
    

    return 0;
}

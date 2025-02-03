#include "LocationConfig.hpp"


void LocationConfig::displayLocation()
{
    std::cout << _path << std::endl;
    std::cout << _root << std::endl;
    std::cout << _index << std::endl;
    UtilParsing::displayVector(_methods);
}
void LocationConfig::controlDefaultLocationConf()
{
    if (_path.empty())
        throw std::invalid_argument("'location' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
    if (_root.empty())
        throw std::invalid_argument("'root' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
    if (_index.empty())
        throw std::invalid_argument("'index' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
    if (_methods.empty())
        throw std::invalid_argument("'methods_accept' must not be empty. Put the keyword (in quotes) followed by its value(s) separated by a space.");
}


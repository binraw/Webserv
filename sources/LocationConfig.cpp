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
        throw std::invalid_argument("path location must not be empty");
    if (_root.empty())
        throw std::invalid_argument("root must not be empty");
    if (_index.empty())
        throw std::invalid_argument("index must not be empty");
    if (_methods.empty())
        throw std::invalid_argument("methods must not be empty");
}


#include "LocationConfig.hpp"


void LocationConfig::displayLocation()
{
    std::cout << _path << std::endl;
    std::cout << _root << std::endl;
    std::cout << _index << std::endl;
    UtilParsing::displayVector(_methods);
}

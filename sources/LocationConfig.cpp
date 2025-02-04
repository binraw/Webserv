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

void LocationConfig::checkSemiColonLocation()
{
    if (_path.find(";") != std::string::npos)
        _path = UtilParsing::trimSemicolon(_path);
    if (_root.find(";") != std::string::npos)
        _root = UtilParsing::trimSemicolon(_root);
    if (_index.find(";") != std::string::npos)
        _index = UtilParsing::trimSemicolon(_index);
    for (std::vector<std::string>::iterator it = _methods.begin(); it != _methods.end(); it++)
    {
        if (it->find(";") != std::string::npos)
            *it = UtilParsing::trimSemicolon(*it);
    }
}


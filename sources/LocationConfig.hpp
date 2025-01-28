#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

#include "ConfigParser.hpp"

class LocationConfig 
{
public:
    std::string path;
    std::string root;
    std::string index;
    std::vector<std::string> methods;

    void addMethod(const std::string& method) 
    {
        methods.push_back(method);
    }
};

#endif

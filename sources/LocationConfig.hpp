#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

#include <vector>
#include "UtilParsing.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <unistd.h>

class LocationConfig 
{
public:
    std::string _path;
    std::string _root;
    std::string _index;
    std::vector<std::string> _methods;

    void addMethod(const std::string& method) 
    {
        _methods.push_back(method);
    }
};

#endif

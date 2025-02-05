#ifndef CGIFACTORY_HPP
# define CGIFACTORY_HPP

#include "CGIScript.hpp"
#include <string>

class CGIFactory
{
private:
    /* data */
public:
    CGIFactory();
    ~CGIFactory();
    CGIScript* createCGIScript(const std::string& type);    
};




#endif
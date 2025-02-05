#include "CGIFactory.hpp"

CGIFactory::CGIFactory()
{
}

CGIFactory::~CGIFactory()
{
}

CGIScript* CGIFactory::createCGIScript(const std::string& type) // ici je peux rajouter un new ... suivant le type 
{
    if (type == "php")
        return new PHPScript();
    else
        return NULL;
}
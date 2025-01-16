#include "Cluster.hpp"

Cluster::Cluster()
{
}

Cluster::~Cluster()
{
}

Cluster::Cluster(const std::string &filename)
{
    std::ifstream inputFile(filename.c_str());
    if (!inputFile.is_open()) 
    {
        std::cerr << "Could not open conf file" << std::endl;
        return;
    }

    std::string line;
    std::map<std::string, std::string> currentConfig;

    while (std::getline(inputFile, line)) 
    {
        if (line.empty() || line[0] == '#') continue;

        if (line.find("server") != std::string::npos) 
        {
            if (!currentConfig.empty()) 
            {
                Server newServer(currentConfig);
                _servers.push_back(newServer);
                currentConfig.clear();
            }
            continue;
        }
        size_t pos = line.find('=');
        if (pos != std::string::npos) 
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            currentConfig[key] = value;
        }
    }
    if (!currentConfig.empty())
    {
        Server newServer(currentConfig);
        _servers.push_back(newServer);
        // currentConfig.clear();
    }

    inputFile.close();
}

void Cluster::initAllServer()
{
    std::cout << "Number of servers: " << _servers.size() << std::endl;
    for(size_t i = 0; i < _servers.size(); i++)
    {
        std::cout << "Server Go: " << std::endl;
        _servers[i].init_data();
        std::cout << "wesh " << std::endl;
    }
}

void Cluster::displayServersConfig()
{
    for(size_t i = 0; i < _servers.size(); i++)
    {
        _servers[i].displayConfig();
    }
}

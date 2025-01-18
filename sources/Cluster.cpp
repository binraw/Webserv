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

        // if (line.find("server") != std::string::npos) 
        // {
        //     if (!currentConfig.empty()) 
        //     {
        //         std::cout << "Valeurs dans la map :" << std::endl;
        //         for (std::map<std::string, std::string>::iterator it = currentConfig.begin(); it != currentConfig.end(); ++it) 
        //         {
        //         std::cout << it->second << std::endl;
        //         }
        //         Server newServer(currentConfig);
        //         _servers.push_back(newServer);
        //         currentConfig.clear();
        //     }
        //     continue;
        // }
         size_t pos = line.find('=');
        // if (pos != std::string::npos) 
        // {
        //     std::string key = line.substr(0, pos);
        //     std::string value = line.substr(pos + 1);
        //     currentConfig[key] = value;
        // }
        if (pos != std::string::npos)
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            currentConfig[key] = value;

            size_t nextPos = line.find('=', pos + 1);
            while (nextPos != std::string::npos)
            {
                key = line.substr(pos + 1, nextPos - pos - 1);
                value = line.substr(nextPos + 1);
                currentConfig[key] = value;
                pos = nextPos;
                nextPos = line.find('=', pos + 1);
            }
        }
        // if (line.find("server") != std::string::npos)
        // {
        //     break;
        // }
    }
    for (std::map<std::string, std::string>::iterator it = currentConfig.begin(); it != currentConfig.end(); ++it) 
    {
    std::cout << it->second << std::endl;
    }
    Server newServer(currentConfig);
    _servers.push_back(newServer);
    currentConfig.clear();
    
    // if (!currentConfig.empty())
    // {
    //     _servers.push_back(Server(currentConfig));
    //     // Server newServer(currentConfig);
    //     // _servers.push_back(newServer);
    //     // currentConfig.clear();
    // }
    _servers[0].displayConfig();
    std::cout << "Number of servers: " << _servers.size() << std::endl;
    inputFile.close();
}

void Cluster::initAllServer()
{
    if (_servers.empty()) {
        std::cerr << "No server configured" << std::endl;
        return;
    }

    std::cout << "Number of servers: " << _servers.size() << std::endl;
    for(size_t i = 0; i < _servers.size(); i++)
    {
        try
        {
            std::cout << "Initialisation du serveur " << i + 1 << ": " << std::endl;
            if (_servers[i].init_data() < 0) {
                std::cerr << "Failed to initialize server " << i + 1 << std::endl;
                continue;
            }
            std::cout << "Server " << i + 1 << " initialized successfully" << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error during server initialization " << i + 1 << ": " << e.what() << std::endl;
        }
    }
}

void Cluster::displayServersConfig()
{
    for(size_t i = 0; i < _servers.size(); i++)
    {
        _servers[i].displayConfig();
    }
}

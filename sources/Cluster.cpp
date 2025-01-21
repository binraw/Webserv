#include "Cluster.hpp"


Cluster::Cluster()
{
}

Cluster::~Cluster()
{
}

Cluster::Cluster(const std::string &filename)
{
    _configPath = filename;
    std::ifstream inputFile(filename.c_str());
    std::string line;
    if (!inputFile.is_open()) 
    {
        std::cerr << "Could not open conf file" << std::endl;
        return;
    }
    while (std::getline(inputFile, line))
    {
        if (line.empty() || line[0] == '#') continue;
        _allConf = UtilParsing::split(line, std::string(" "));
    }
    initDefaultConf();
    initAllServer();
    inputFile.close();
}

// Ici je recolte les servers que je mets dans une map pour l'instand un seul est enregistrer 

void Cluster::initDefaultConf()
{
    std::vector<std::string>::iterator it;
    int i = 0;
    int number_servers = 0;
    for(it = _allConf.begin(); it != _allConf.end(); it++)
    {
        if (*it == std::string("{"))
            i++;
        if (*it == std::string("server")) // il faut que ca rentre a chaque nouveau server
        {
            std::cout << " RENTRE " << std::endl;
            //faut utiliser pair pour creer des paire comme ca avec des maps tres style
            std::pair<int, std::vector<std::string> > serverPair(number_servers, addValuesServers(it));
            _vectServers.insert(serverPair);
            _defaultConf.pop_back();
            number_servers++;
        }
        if (*it == std::string("}"))
            i--;
        if (i <= 1)
            _defaultConf.push_back(*it);
    }
    //   for (size_t y = 0; y < _defaultConf.size(); y++) {
    //     std::cout << _defaultConf[y] << std::endl;
    // }

  for (std::map<int, std::vector<std::string> >::const_iterator tic = _vectServers.begin(); tic != _vectServers.end(); ++tic) {
        std::cout << "Server Number: " << tic->first << "\nConfigurations:\n";
        
        // Itération sur le vecteur de configurations
        for (std::vector<std::string>::const_iterator configIt = tic->second.begin(); configIt != tic->second.end(); ++configIt) {
            std::cout << "  - " << *configIt << "\n"; // Affiche chaque configuration
        }
    }

}

std::vector<std::string> Cluster::addValuesServers(std::vector<std::string>::iterator &cursor)
{
    std::vector<std::string> server;
    std::vector<std::string>::iterator it;
    server.push_back(*cursor);
    int i = 0;
    int y = 0;
    for(it = cursor;it != _allConf.end(); it++)
    {
        if (*it == std::string("{"))
            i++;
        if (*it == std::string("}"))
            i--;
        if (i > 0)
            server.push_back(*it);
        if (i == 0 && y > 0)
            break;
        y++;
    }
    // std::cout << "Value du serveur :" << std::endl;
    // for (size_t y = 0; y < server.size(); y++) 
    // {
    //     std::cout << server[y] << std::endl;
    // }
    return server;
}

void Cluster::initAllServer()
{
    for(std::map<int, std::vector<std::string> >::const_iterator it = _vectServers.begin(); it != _vectServers.end(); it++)
    {
       _servers.push_back(Server(it->second));
    }
}


// Cluster::Cluster(const std::string &filename)
// {

//     // char *str;

//     // try
//     // {
//     //     str = new char [10];
//     // }
//     // catch(const std::bad_alloc& e)
//     // {
//     //     std::cerr << e.what() << '\n';
//     // }
    
//     std::ifstream inputFile(filename.c_str());
//     if (!inputFile.is_open()) 
//     {
//         std::cerr << "Could not open conf file" << std::endl;
//         return;
//     }

//     std::string line;
//     std::map<std::string, std::string> currentConfig;

//     while (std::getline(inputFile, line)) 
//     {
//         if (line.empty() || line[0] == '#') continue;

//         // if (line.find("server") != std::string::npos) 
//         // {
//         //     if (!currentConfig.empty()) 
//         //     {
//         //         std::cout << "Valeurs dans la map :" << std::endl;
//         //         for (std::map<std::string, std::string>::iterator it = currentConfig.begin(); it != currentConfig.end(); ++it) 
//         //         {
//         //         std::cout << it->second << std::endl;
//         //         }
//         //         Server newServer(currentConfig);
//         //         _servers.push_back(newServer);
//         //         currentConfig.clear();
//         //     }
//         //     continue;
//         // }
//          size_t pos = line.find('=');
//         // if (pos != std::string::npos) 
//         // {
//         //     std::string key = line.substr(0, pos);
//         //     std::string value = line.substr(pos + 1);
//         //     currentConfig[key] = value;
//         // }
//         if (pos != std::string::npos)
//         {
//             std::string key = line.substr(0, pos);
//             std::string value = line.substr(pos + 1);
//             currentConfig[key] = value;

//             size_t nextPos = line.find('=', pos + 1);
//             while (nextPos != std::string::npos)
//             {
//                 key = line.substr(pos + 1, nextPos - pos - 1);
//                 value = line.substr(nextPos + 1);
//                 currentConfig[key] = value;
//                 pos = nextPos;
//                 nextPos = line.find('=', pos + 1);
//             }
//         }
//         // if (line.find("server") != std::string::npos)
//         // {
//         //     break;
//         // }
//     }
//     for (std::map<std::string, std::string>::iterator it = currentConfig.begin(); it != currentConfig.end(); ++it) 
//     {
//     std::cout << it->second << std::endl;
//     }
//     Server newServer(currentConfig);
//     _servers.push_back(newServer);
//     currentConfig.clear();
    
//     // if (!currentConfig.empty())
//     // {
//     //     _servers.push_back(Server(currentConfig));
//     //     // Server newServer(currentConfig);
//     //     // _servers.push_back(newServer);
//     //     // currentConfig.clear();
//     // }
//     _servers[0].displayConfig();
//     std::cout << "Number of servers: " << _servers.size() << std::endl;
//     inputFile.close();
// }

// void Cluster::initAllServer()
// {
//     if (_servers.empty()) {
//         std::cerr << "No server configured" << std::endl;
//         return;
//     }

//     std::cout << "Number of servers: " << _servers.size() << std::endl;
//     for(size_t i = 0; i < _servers.size(); i++)
//     {
//         try
//         {
//             std::cout << "Initialisation du serveur " << i + 1 << ": " << std::endl;
//             if (_servers[i].init_data() < 0) {
//                 std::cerr << "Failed to initialize server " << i + 1 << std::endl;
//                 continue;
//             }
//             std::cout << "Server " << i + 1 << " initialized successfully" << std::endl;
//         }
//         catch (const std::exception& e)
//         {
//             std::cerr << "Error during server initialization " << i + 1 << ": " << e.what() << std::endl;
//         }
//     }
// }

// void Cluster::displayServersConfig()
// {
//     for(size_t i = 0; i < _servers.size(); i++)
//     {
//         _servers[i].displayConfig();
//     }
// }

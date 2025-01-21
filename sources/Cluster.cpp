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

// Ici je recolte les servers que je mets dans une map 

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


//   for (std::map<int, std::vector<std::string> >::const_iterator tic = _vectServers.begin(); tic != _vectServers.end(); ++tic) {
//         std::cout << "Server Number: " << tic->first << "\nConfigurations:\n";
        
//         // Itération sur le vecteur de configurations
//         for (std::vector<std::string>::const_iterator configIt = tic->second.begin(); configIt != tic->second.end(); ++configIt) {
//             std::cout << "  - " << *configIt << "\n"; // Affiche chaque configuration
//         }
//     }

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


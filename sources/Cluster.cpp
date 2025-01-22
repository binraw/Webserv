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
        if (*it == std::string("server"))
        {
            //faut utiliser pair pour creer des paire comme ca avec des maps tres style
            std::pair<int, std::vector<std::string> > serverPair(number_servers, addValuesServers(it));
            _vectServers.insert(serverPair);
            // _defaultConf.pop_back();  j enleve je sais pas si je casse tout
            number_servers++;
        }
        if (*it == std::string("}"))
            i--;
        if (i <= 1)
            _defaultConf.push_back(*it);
    }
    cleanClusterConfDefault();
    // for (std::vector<std::string>::iterator t = _defaultConf.begin(); t != _defaultConf.end(); t++)
    // {
    //     std::cout << *t << std::endl;
    // }

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

void Cluster::cleanClusterConfDefault()
{
    _defaultConf = UtilParsing::cleanVector(_defaultConf);
    for (std::vector<std::string>::iterator it = _defaultConf.begin(); it != _defaultConf.end(); )
    {
        if (*it == "{" || *it == "}" || *it == "http" || *it == "server") 
        { 
            it = _defaultConf.erase(it);
        } else 
            ++it;
    }
}


// reussir a faire une map organiser des valeurs default de cluster 
// la trouver un algo pour check quand il y a un ; et si celui d'avant et encore avant en a 
// genre si jai pas jai pas et apres jai alors donner la key au premier 
// et creer une string composer des autres 
// doit etre modulable si y a 5 argument avec les methodes par exemple par la suite

// void Cluster::createMapDefaultConf()
// {
//     // std::string key;
//     // std::string params;
//     // size_t i = 0;
//     // for (std::vector<std::string>::iterator it = _defaultConf; it != _defaultConf.end(); )
//     // {
//     //     if (i == 0 && !(*it.find(";"))) // pour la premiere key


//     // }
// }

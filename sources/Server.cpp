#include "../headers/Server.hpp"

Server::Server(const std::string& filename)
{
     _params_config = parseConf(filename);
    // init_data("server.conf");
}

Server::~Server()
{
}

void Server::init_data(const std::string& filename)
{
    std::map<std::string, std::string> config = parseConf(filename);
    // if (config.find("port") != config.end()) 
    //     port = config.at("port");
    // if (config.find("server_name") != config.end()) 
    //     server_name = config.at("server_name");
    // if (config.find("root") != config.end()) 
    //     root = config.at("root");
    // if (config.find("index") != config.end()) 
    //     index = config.at("index");
    // if (config.find("error_page404") != config.end()) 
    //     error_page404 = config.at("error_page404");
    // if (config.find("client_max_body_size") != config.end()) 
    //     client_max_body_size = config.at("client_max_body_size");
    // if (config.find("cgi_path") != config.end()) 
    //     cgi_path = config.at("cgi_path");
    // if (config.find("upload_path") != config.end()) 
    //     upload_path = config.at("upload_path");
    // if (config.find("autoindex") != config.end()) 
    //     autoindex = config.at("autoindex");
    // if (config.find("limit_except") != config.end()) 
    //     limit_except = config.at("limit_except");
}


std::map<std::string, std::string> Server::parseConf(const std::string& filename)
{
    std::map<std::string, std::string> conf;
    std::string line;
    std::string content;
    std::ifstream inputFile(filename.c_str());
    if (!inputFile.is_open())
    {
        std::cerr << "Could not open conf file" << std::endl;
        return conf;
    }
    while(std::getline(inputFile, line))
    {
        if (line.empty() || line[0] == '#') continue;

        size_t pos = line.find('=');
        if (pos != std::string::npos)
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            conf[key] = value;

            size_t nextPos = line.find('=', pos + 1);
            while (nextPos != std::string::npos)
            {
                key = line.substr(pos + 1, nextPos - pos - 1);
                value = line.substr(nextPos + 1);
                conf[key] = value;
                pos = nextPos;
                nextPos = line.find('=', pos + 1);
            }
        }
    }
    inputFile.close();
    return conf;
}

// void Server::displayConfig()
// {
//     std::cout << "Server Name: " << server_name << std::endl;
//     std::cout << "Port: " << port << std::endl;
//     std::cout << "Root Directory: " << root << std::endl;
//     std::cout << "Index: " << index << std::endl;
//     std::cout << "Error Page 404: " << error_page404 << std::endl;
//     std::cout << "Client Max Body Size: " << client_max_body_size << std::endl;
//     std::cout << "CGI Path: " << cgi_path << std::endl;
//     std::cout << "Upload Path: " << upload_path << std::endl;
//     std::cout << "Autoindex: " << autoindex << std::endl;
//     std::cout << "Limit Except: " << limit_except << std::endl;
// }


void Server::displayConfig()
{
    std::cout << "Server Name: " << _params_config.at("server_name") << std::endl;
    std::cout << "Port: " << _params_config.at("port") << std::endl;
    std::cout << "Root Directory: " << _params_config.at("root") << std::endl;
    std::cout << "Index: " << _params_config.at("index") << std::endl;
    std::cout << "Error Page 404: " << _params_config.at("error_page404") << std::endl;
    std::cout << "Client Max Body Size: " << _params_config.at("client_max_body_size") << std::endl;
    std::cout << "CGI Path: " << _params_config.at("cgi_path") << std::endl;
    std::cout << "Upload Path: " << _params_config.at("upload_path") << std::endl;
    std::cout << "Autoindex: " << _params_config.at("autoindex") << std::endl;
    std::cout << "Limit Except: " << _params_config.at("limit_except") << std::endl;
}

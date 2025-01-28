#include "ConfigParser.hpp"

HttpConfig ConfigParser::parse(const std::string& filepath) 
{
    HttpConfig httpConfig;
    std::ifstream file(filepath.c_str());
    if (!file.is_open()) 
    {
        throw std::runtime_error("Could not open config file");
    }

    std::string line;
    while (std::getline(file, line)) 
    {
        if (line.empty() || line[0] == '#') continue;

        if (line == "http {") 
        {
            parseHttpBlock(file, httpConfig);
        }
    }
    
    return httpConfig;
}

void ConfigParser::parseHttpBlock(std::ifstream& file, HttpConfig& httpConfig) 
{
    std::string line;
    while (std::getline(file, line)) 
    {
        if (line.find("include") != std::string::npos)
        {
            _include.push_back(line.substr(line.find(' ') + 1));
        }
        if (line.find("default_type") != std::string::npos)
        {
            _default_type = line.substr(line.find(' ') + 1);
        }
        if (line.find("keepalive_timeout") != std::string::npos)
        {
            _keepalive_timeout = line.substr(line.find(' ') + 1);
        }
        if (line.find("worker_connexion") != std::string::npos)
        {
            _worker_connexion = line.substr(line.find(' ') + 1);
        }
        if (line == "server {") 
        {
            ServerConfig serverConfig;
            parseServerBlock(file, serverConfig);
            httpConfig.servers.push_back(serverConfig);
        } 
        else if (line == "}") 
        {
            break;
        }
    }
}

void ConfigParser::parseServerBlock(std::ifstream& file, ServerConfig& serverConfig) 
{
    std::string line;
    while (std::getline(file, line)) 
    {
        if (line.find("server_name") != std::string::npos) 
        {
            serverConfig.serverName.push_back(line.substr(line.find(' ') + 1));
        } 
        else if (line.find("listen") != std::string::npos) 
        {
            serverConfig.listenPort.push_back(line.substr(line.find(' ') + 1));
        } 
        else if (line.find("client_max_body_size") != std::string::npos) 
        {
            serverConfig.clientMaxBodySize = line.substr(line.find(' ') + 1);
        } 
        else if (line.find("upload_path") != std::string::npos) 
        {
            serverConfig.uploadPath = line.substr(line.find(' ') + 1);
        } 
        else if (line.find("location") != std::string::npos) 
        {
            LocationConfig locationConfig;
            locationConfig.path = line.substr(line.find(' ') + 1);
            parseLocationBlock(file, locationConfig);
            serverConfig.locations.push_back(locationConfig);
        } 
        else if (line == "}") 
        {
            break;
        }
    }
}

void ConfigParser::parseLocationBlock(std::ifstream& file, LocationConfig& locationConfig) 
{
    std::string line;
    while (std::getline(file, line)) 
    {
        if (line.find("root") != std::string::npos) 
        {
            locationConfig.root = line.substr(line.find(' ') + 1);
        } 
        else if (line.find("index") != std::string::npos) 
        {
            locationConfig.index = line.substr(line.find(' ') + 1);
        } 
        else if (line.find("methods_accept") != std::string::npos) 
        {
            std::istringstream iss(line.substr(line.find(' ') + 1));
            std::string method;
            while (iss >> method) 
            {
                locationConfig.addMethod(method);
            }
        } 
        else if (line == "}") 
        {
            break;
        }
    }
}

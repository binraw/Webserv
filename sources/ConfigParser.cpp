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
            httpConfig._include = (UtilParsing::splitSpecialDeleteKey(line, std::string(" ")));
        }
        if (line.find("default_type") != std::string::npos)
        {
            httpConfig._default_type = UtilParsing::recoverValue(line, "default_type");
        }
        if (line.find("keepalive_timeout") != std::string::npos)
        {
            httpConfig._keepalive_timeout = UtilParsing::recoverValue(line, "keepalive_timeout");
        }
        if (line.find("worker_connexion") != std::string::npos)
        {
            httpConfig._worker_connexion = UtilParsing::recoverValue(line, "worker_connexion");
        }
        if (line.find("server") != std::string::npos) 
        {
            ServerConfig serverConfig;
            parseServerBlock(file, serverConfig);
            httpConfig._servers.push_back(serverConfig);
        } 
        // else if (line == "}") 
        // {
        //     break;
        // }
    }
}

void ConfigParser::parseServerBlock(std::ifstream& file, ServerConfig& serverConfig) 
{
    std::string line;
    while (std::getline(file, line)) 
    {
        if (line.find("server_name") != std::string::npos) 
        {
            serverConfig._serverName = (UtilParsing::splitSpecialDeleteKey(line, std::string(" ")));
        } 
        else if (line.find("listen") != std::string::npos) 
        {
            serverConfig._listenPort = (UtilParsing::splitSpecialDeleteKey(UtilParsing::trim(line), std::string(" ")));
        } 
        else if (line.find("client_max_body_size") != std::string::npos) 
        {
            serverConfig._clientMaxBodySize = UtilParsing::recoverValue(line, "client_max_body_size");
        } 
        else if (line.find("upload_path") != std::string::npos) 
        {
            serverConfig._uploadPath = UtilParsing::recoverValue(line, "upload_path");
        } 
        else if (line.find("location") != std::string::npos) 
        {
            LocationConfig locationConfig;
            locationConfig._path = line.substr(line.find(' ') + 1);
            parseLocationBlock(file, locationConfig);
            serverConfig._locations.push_back(locationConfig);
        } 
        else if (line.find("}")) 
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
            locationConfig._root = UtilParsing::recoverValue(line, "root");
        } 
        else if (line.find("index") != std::string::npos) 
        {
            locationConfig._index = UtilParsing::recoverValue(line, "index");
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
        else if (line.find("}")) 
        {
            break;
        }
    }
}




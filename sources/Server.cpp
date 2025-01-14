#include "../headers/Server.hpp"

Server::Server(const std::string& filename)
{
     _paramsConfig = parseConf(filename);
}

Server::~Server()
{
}

void Server::init_data()
{
    this->_socket = Socket ref(this->_paramsConfig);
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

int Server::controlMap()
{
  std::vector<std::string> requiredKeys;
    requiredKeys.push_back("server_name");
    requiredKeys.push_back("port");
    requiredKeys.push_back("root");
    requiredKeys.push_back("index");
    requiredKeys.push_back("error_page404");
    requiredKeys.push_back("client_max_body_size");
    requiredKeys.push_back("cgi_path");
    requiredKeys.push_back("upload_path");
    requiredKeys.push_back("autoindex");
    requiredKeys.push_back("limit_except");

    for (std::vector<std::string>::const_iterator it = requiredKeys.begin(); it != requiredKeys.end(); ++it) 
    {
        std::map<std::string, std::string>::const_iterator paramIt = _paramsConfig.find(*it);
        if (paramIt == _paramsConfig.end() || paramIt->second.empty())
            return (-1);
    }
    int port;
    std::istringstream iss(_paramsConfig.at("port"));
    if (!iss)
        return (-1);
    iss >> port;
    if (port < 1024 || port > 65535)
        return (-1);
    return (0);
}

const char* Server::ErrorConfigParam::what() const throw()
{
    return "Missing or empty configuration parameter";
}


void Server::displayConfig()
{
    std::cout << "Server Name: " << _paramsConfig.at("server_name") << std::endl;
    std::cout << "Port: " << _paramsConfig.at("port") << std::endl;
    std::cout << "Root Directory: " << _paramsConfig.at("root") << std::endl;
    std::cout << "Index: " << _paramsConfig.at("index") << std::endl;
    std::cout << "Error Page 404: " << _paramsConfig.at("error_page404") << std::endl;
    std::cout << "Client Max Body Size: " << _paramsConfig.at("client_max_body_size") << std::endl;
    std::cout << "CGI Path: " << _paramsConfig.at("cgi_path") << std::endl;
    std::cout << "Upload Path: " << _paramsConfig.at("upload_path") << std::endl;
    std::cout << "Autoindex: " << _paramsConfig.at("autoindex") << std::endl;
    std::cout << "Limit Except: " << _paramsConfig.at("limit_except") << std::endl;
}

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>

class Server
{
private:
    std::string port;
    std::string server_name;
    std::string root;
    std::string index;
    std::string error_page404;
    std::string client_max_body_size;
    std::string cgi_path;
    std::string upload_path;
    std::string autoindex;
    std::string limit_except; 

    
public:
    Server();
    ~Server();
    void init_data(const std::string& filename_config);
    std::map<std::string, std::string> parseConf(const std::string& filename);
    void displayConfig();
};



#endif

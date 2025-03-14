#ifndef CGI_HPP
# define CGI_HPP

#include "Client.hpp"
#include <cstring>
#include <stdlib.h>
#include <dirent.h>

void processCGI(const std::string &path, Server server, Request req);
bool checkExtensionCGI(const std::string &path, Server server);
std::string extractDirectory(const std::string &path);
bool moveToDirectoryScript(const std::string &directory);
char** initEnv(Request req, Server server);
std::string playCgi(const std::string &path, Request req, char **env);
std::string executeCGI(const std::string &path, Server server, Request req);
void freeEnv(char** tab);
std::string ParseUri(std::string uri);
void childProcessCgi(char**env, int *pipe_in, int *pipe_out);
void childProcessCgiPy(char**env, int *pipe_in, int *pipe_out);
std::string parentProcessCgi(Request req, pid_t pid, int *pipe_in, int *pipe_out);
std::string createBody(int *pipe_out);
void closePipe(int *pipe_in, int *pipe_out);
int controlContentBodyReq(Request req);

// ----------------AUTOINDEX PART ----------------------------

int controlAutoIndex(Server server, std::string path);
std::string processAutoIndex(Server server, std::string path);
std::string buildBodyAutoIndex(DIR *dir, std::string path);
std::string buildHeaderAutoIndex(std::string body);
std::string processAutoIndexTEST(std::string path);

//------------------PARTIE TEST EN DUR-------------------------

void processCGITEST(const std::string &path);
bool checkExtensionCGITEST(const std::string &path);
std::string executeCGITEST(const std::string &path);
char** initEnvTEST();
std::string playCGITEST(const std::string path, char** env);

std::string executeCgiTEST(char **env) ;




class ErrorCGI : virtual public std::exception 
{
    public:
        ErrorCGI(const std::string &message, int code) : _message(message), _code(code){}
        virtual const char *	what() const throw() 
        {
            (void) _code;
             return _message.c_str();
        }
        virtual ~ErrorCGI() throw() {};

    private:
        std::string _message;
        int         _code;
};

#endif

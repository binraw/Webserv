#ifndef CGI_HPP
# define CGI_HPP

#include "Client.hpp"
#include <cstring>
#include <stdlib.h>

void processCGI(const std::string &path, Server server, Request req);
// bool checkExtensionCGI(const std::string &path, Server server);
std::string extractDirectory(const std::string &path);
bool moveToDirectoryScript(const std::string &directory);
char** initEnv(Request req, Server server);
std::string playCgi(const std::string &path, Request req, char **env);
std::string executeCGI(const std::string &path, Server server, Request req);
void freeEnv(char** tab);
std::string ParseUri(std::string uri);
void childProcessCgi(char**env, int *pipe_in, int *pipe_out);
std::string parentProcessCgi(Request req, pid_t pid, int *pipe_in, int *pipe_out);
std::string createBody(int *pipe_out);
void closePipe(int *pipe_in, int *pipe_out);
int controlContentBodyReq(Request req);

//------------------PARTIE TEST EN DUR-------------------------

void processCGITEST(const std::string &path);
bool checkExtensionCGITEST(const std::string &path);
std::string executeCGITEST(const std::string &path);
char** initEnvTEST();
std::string playCGITEST(const std::string path, char** env);

std::string executeCgiTEST(char **env) ;

#endif

#include "Client.hpp"
#include <cstring>
#include "CGI.hpp"

// -------------------------ICI COMMENCE LA PARTIE TEST----------------------------


// void processCGITEST(const std::string &path)
// {
//     std::string response;
//     try 
//     {
//         if (UtilParsing::fileExits(path) != true) //ici voir mais a enlever le check normalement
//             throw ErrorCGI("Not found", 404);
//         if (access(path.c_str(), X_OK) != 0)
//             throw ErrorCGI("Not found", 404);
//         if (checkExtensionCGITEST(path) != true)
//             throw ErrorCGI("Bad Gateway", 502);
//         if (moveToDirectoryScript(extractDirectory(path)) != true)
//             throw ErrorCGI("Internal server error", 500);
//         response = executeCGITEST(path); 
//         if (response.empty())
//             throw ErrorCGI("Bad Gateway", 502);
//     }
//     catch (const ErrorCGI& e)
//     {
//         std::cerr << e.what() << std::endl;
//     }
//     std::cout << response << std::endl;
// }


// ici on pourra rajouter d'autres option si on veut faire fonctionner d'autre type de CGI
bool checkExtensionCGITEST(const std::string &path)
{
   std::string cgi_path = "support.pl";

    if (UtilParsing::recoverExtension(path) == UtilParsing::recoverExtension(cgi_path))
        return true;
    return false;
}

// std::string executeCGITEST(const std::string &path)
// {
//     char **env;
//     std::string body;
//     env = initEnvTEST();
//     body = playCGITEST(path, env);
//     freeEnv(env);
//     return body;
// }

char** initEnvTEST()
{
      std::string environnement[] = {
        "REQUEST_METHOD=POST",
        "CONTENT_LENGTH=1000",
        "QUERY_STRING=value1=35",
        "CONTENT_TYPE=application/x-www-form-urlencoded",
        "HTTP_HOST=localhost",
        "SCRIPT_NAME=script.pl",
        "PATH_INFO=./cgi-bin/script.pl", 
    };
    int  environSize = sizeof(environnement) / sizeof(environnement[0]);
    char** environTEST = new char*[environSize + 1]; 

    for (int i = 0; i < environSize; i++) 
    {
        environTEST[i] = new char[environnement[i].size() + 1];
        strcpy(environTEST[i], environnement[i].c_str());
    }
    environTEST[environSize] = NULL;

    return environTEST;
}


// int childProcessPostTEST(const std::string path, char**env, int *pipfd)
// {
//     const char *filename;
//     std::FILE *tempfile = std::tmpfile();
//     long fdIn = fileno(tempfile);
//     std::string body = "{\"name\": \" John Doe\"}";
//     write(fdIn, body.c_str(), body.size());
//     lseek(fdIn, 0, SEEK_SET); // la je ne suis pas sur de devoir remettre a 0 apes avoir ecrit
//     dup2(pipfd[0], fdIn); 
//     close(pipfd[0]);
//     close(fdIn);
//     dup2(pipfd[1], STDOUT_FILENO); 
//     close(pipfd[1]);
//     std::string::size_type start = path.find_last_of("/");
//     if (start != std::string::npos)
//         filename = path.substr(start + 1).c_str();
//     else
//         filename = path.c_str();
//     const char *args[] = {"/usr/bin/perl", filename, NULL};
//     if (execve(args[0],(char* const*)args , env) == -1)
//         return 1;
//     return 0;
// }


// int childProcessPostTEST(const std::string path, char**env, int *pipfd)
// {
//     const char *filename;
//     std::FILE *tempfile = std::tmpfile();
//     long fdIn = fileno(tempfile);
//     std::string body = "{\"name\": \" John Doe\"}";
//     write(fdIn, body.c_str(), body.size());
//     close(fdIn);
//     lseek(pipfd[0], 0, SEEK_SET);
//     dup2(pipfd[0], STDIN_FILENO);
//     close(pipfd[0]);
//     dup2(pipfd[1], STDOUT_FILENO); 
//     close(pipfd[1]);
//     std::string::size_type start = path.find_last_of("/");
//     if (start != std::string::npos)
//         filename = path.substr(start + 1).c_str();
//     else
//         filename = path.c_str();
//     const char *args[] = {"/usr/bin/perl", filename, NULL};
//     if (execve(args[0],(char* const*)args , env) == -1)
//         return 1;
//     return 0;
// }




// std::string playCGITEST(const std::string path, char** env)
// {
//     std::string output;
//     int pipfd[2];
//     int			saveStdin;
// 	int			saveStdout;
//     saveStdin = dup(STDIN_FILENO);
// 	saveStdout = dup(STDOUT_FILENO);

//     if (pipe(pipfd) == -1)
//         return "";
//     pid_t pid = fork();
//     if (pid < 0)
//     {
//         close(pipfd[0]);
//         close(pipfd[1]);
//         return "";
//     }
//     else if (pid == 0) 
//         childProcessPostTEST(path, env, pipfd);
//     else 
//         output = parentProcess(pid, pipfd);
//     dup2(saveStdin, STDIN_FILENO);
// 	dup2(saveStdout, STDOUT_FILENO);
//     close(saveStdin);
// 	close(saveStdout);

//     return output;
// }

// std::string ParseBodyPost(std::string body, std::string contentType)
// {
//     if (contentType == "application/x-ww-form-urllencoded")
//         return body;
//     else if (contentType == "multipart/form-data")
//         return ParseMultipart(body, "weeeeeeessssssssssh");
//     else if (contentType == "text/plain")
//         return ParseText(body);
//     else
//         return body;
// }

// std::string ParseMultipart(std::string body, std::string boundary)
// {


// }


// std::string ParseText(std::string body)
// {
//     return body;
// }


// recuperation d'un code pour pouvoir comparer :


std::string executeCgiTEST(char **env) 
{
    int pipe_in[2];
    int pipe_out[2];
    pid_t pid;
    std::string newBody;
    std::string body = "objectif=Creation+de+site+web+pour+entreprise&design=oui&rdv=non";

    if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) 
    {
        return "Status: 500\r\n\r\n";
    }

    pid = fork();
    if (pid == -1) 
    {
        close(pipe_in[0]); close(pipe_in[1]);
        close(pipe_out[0]); close(pipe_out[1]);
        return "Status: 500\r\n\r\n";
    }

    if (pid == 0)
    { 
        close(pipe_in[1]);
        close(pipe_out[0]);
        dup2(pipe_in[0], STDIN_FILENO);
        close(pipe_in[0]);
        dup2(pipe_out[1], STDOUT_FILENO);
        close(pipe_out[1]);
        const char *args[] = {"/usr/bin/perl", "./cgi-bin/scriptQuery.pl", NULL};
        execve(args[0], (char *const *)args, env);
        _exit(1); // ici gestion d'erreur 
    }
    else 
    {
        close(pipe_in[0]);
        close(pipe_out[1]);
        write(pipe_in[1], body.c_str(), body.length());
        close(pipe_in[1]);
        char buffer[4096];
        ssize_t bytes_read;   
        while ((bytes_read = read(pipe_out[0], buffer, sizeof(buffer) - 1)) > 0) 
        {
            buffer[bytes_read] = '\0';
            newBody.append(buffer);
        }
        close(pipe_out[0]);
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            return "Status: 500\r\n\r\n";
    }

    return newBody;
}


// ancien mais on sait jamais ....

// route pour effectuer le pipe + fork neccessaire pour l'execution 
// si erreur on renvoie juste un string vide car le controle se fait plus haut
// std::string playCGI(const std::string path, char** env)
// {
//     std::string output;
//     int pipfd[2];

//     if (pipe(pipfd) == -1)
//         return "";
//     pid_t pid = fork();
//     if (pid < 0)
//     {
//         close(pipfd[0]);
//         close(pipfd[1]);
//         return "";
//     }
//     else if (pid == 0)
//         childProcess(path, env, pipfd);

//     else 
//         output = parentProcess(pid, pipfd);

//     return output;
// }


// je dois enfaite creer un fichier tempo
// qui va avoir tout lee body de la request POST

// int childProcessPostTEST(const std::string path, char**env, int *pipfd)
// {
//     const char *filename;
//     std::FILE *tempfile = std::tmpfile();
//     long fdIn = fileno(tempfile);
//     std::string body = "{\"name\": \" John Doe\"}";
//     write(fdIn, body.c_str(), body.size());
//     lseek(fdIn, 0, SEEK_SET);
//     close(fdIn); // Fermer fdIn après avoir écrit les données

//     dup2(pipfd[0], STDIN_FILENO); // Dupliquer pipfd[0] sur STDIN_FILENO
//     close(pipfd[0]); // Fermer pipfd[0]
//     dup2(pipfd[1], STDOUT_FILENO); // Dupliquer pipfd[1] sur STDOUT_FILENO
//     close(pipfd[1]); // Fermer pipfd[1]

//     std::string::size_type start = path.find_last_of("/");
//     if (start != std::string::npos)
//         filename = path.substr(start + 1).c_str();
//     else
//         filename = path.c_str();
//     const char *args[] = {"/usr/bin/perl", filename, NULL};
//     if (execve(args[0],(char* const*)args , env) == -1)
//         return 1;
//     return 0;
// }


// ici avec les arg dans query_string
// int childProcess(const std::string path, char**env, int *pipfd)
// {
//     const char *filename;

//     close(pipfd[0]);
//     dup2(pipfd[1], STDOUT_FILENO); 
//     close(pipfd[1]);
//     std::string::size_type start = path.find_last_of("/");
//     if (start != std::string::npos)
//         filename = path.substr(start + 1).c_str();
//     else
//         filename = path.c_str();
//     const char *args[] = {"/usr/bin/perl", filename, NULL};
//     if (execve(args[0],(char* const*)args , env) == -1)
//         return 1;
//     return 0;
// }
// le process parent qui va ecrire dans output le retour du script qui dans l'idee 
// s'occupe de creer un body html
// std::string parentProcess(pid_t pid, int *pipfd)
// {
//     std::string output;
//     char buffer[128];
//     ssize_t bytesRead;
//     close(pipfd[1]);
//     while ((bytesRead = read(pipfd[0], buffer, sizeof(buffer) - 1)) > 0)
//     {
//         buffer[bytesRead] = '\0';
//         output.append(buffer);
//     }
//     close(pipfd[0]);
//     int status;
//     waitpid(pid, &status, 0);
//     if (WIFEXITED(status))
//         return output;
//     else 
//         return "";
// }
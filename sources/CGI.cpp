// check l'existance du file a l'endroit donner (on a deja la fct)
// check si le script demander est executable avant tout (avec access)
// verifier aussi l'extension appeler est celle gerer par le server
// se deplacer dans le bon repertoire avant d'exec le script

// construire le header ? ou il sera construit dans les cgi ? 
// je pense qu'il faut en formatter un par default
// une possibilite de creer un fichier env ? pas sur de cette info ca serait sans doute 
//un truc en plus a pouvoir 
// set mais bon dans le sujet pas tres utile

// la fragmentation sera fais avant ou je dois l'implementer dans le processus question a explorer

// infos trouvees:
// Tous les segments de chemin restants sont transmis au script dans la variable d'environnement PATH_INFO.

// Le mot-clé chunked dans l' en-tête Transfer-Encoding est utilisé pour indiquer un transfert fragmenté.
//La transmission se termine lorsqu'un fragment de longueur nulle est reçu.
// la fonction: fcntl peut etre utile si il faut bloquer l'acces d'un fichier pendant un processus
// mais actuellement je sais pas si c'est vraiment utile


// apres des recherche je trouve qu'il faut justement defenir notre propre environnement avec les info envoyer 
// du processus parent pour le processus enfant.
// les diffenrente values presente pour l'env serait : 
// REQUEST_METHOD: Spécifie la méthode HTTP utilisée pour accéder au script
// QUERY_STRING: Contient les données de la requête HTTP sous forme de chaîne de caractères 
// (par exemple, param1=value1&param2=value2)
// CONTENT_TYPE: Spécifie le type de contenu de la requête HTTP
// CONTENT_LENGTH: Spécifie la longueur des données de la requête HTTP
// HTTP_HOST: Spécifie le nom d'hôte du serveur Web
// SCRIPT_NAME: Spécifie le nom du script CGI
// PATH_INFO: Spécifie les informations de chemin supplémentaires qui ont été passées au script
// (par exemple, /path/to/resource)


// ces 3 la je ne sais pas si on les rajoute car je crois qu'ils optionnel
// HTTP_REFERER: Spécifie l'URL de la page Web qui a envoyé la requête
// REMOTE_ADDR: Spécifie l'adresse IP du client qui a envoyé la requête
// REMOTE_PORT: Spécifie le port utilisé par le client pour envoyer la requête

// si un probleme d'affichage pour le retour du script ne pas oubier de verifier le content length
#include "Client.hpp"
#include <cstring>
#include "CGI.hpp"




// ici voir si il faut renvoyer une page error dans le catch
void processCGI(const std::string &path, Server server, Request req)
{
    std::string response;
    try
    {
        if (UtilParsing::fileExits(path) != 0)
            throw ErrorCGI("Not found", 404);
        if (access(path.c_str(), X_OK) != 0)
            throw ErrorCGI("Not found", 404);
        if (checkExtensionCGI(path, server) != 0)
            throw ErrorCGI("Bad Gateway", 502);
        if (moveToDirectoryScript(extractDirectory(path)) != 0)
            throw ErrorCGI("Internal server error", 500);
        response = executeCGI(path, server, req); // ici plus tard renvoyer directement dans la value response
        if (response.empty())
            throw ErrorCGI("Bad Gateway", 502);
    }
    catch (const ErrorCGI& e)
    {
         std::cerr << e.what() << std::endl;
    }
}
// fonction pour check le type d'extension du script demander pour verifier si le server la supporte
// pareil la recuperer dans le parsing le nom par server  le nom du script cgi pour verifier son extension
bool checkExtensionCGI(const std::string &path, Server server)
{
    (void)server;
   std::string cgi_path = "support.pl";

    if (UtilParsing::recoverExtension(path) == UtilParsing::recoverExtension(cgi_path))
        return true;
    return false;
}

// fonction pour extract le nom du dossier
std::string extractDirectory(const std::string &path)
{
    std::string directory;

    size_t posEndDirectory = path.find_last_of("/");
    directory = path.substr(0, posEndDirectory);

    return directory;
}

// fonction qui permet d'aller dans le bon dossier avec execution du processus cgi
bool moveToDirectoryScript(const std::string &directory)
{
    if (chdir(directory.c_str()) != 0)
        return false;
    return true;
}


std::string _method,  _params, _contentType, _http, _httpReferer, _remoteAddr, _remotePort, _scriptName, _pathInfo;
char** initEnv(Request req, Server server)
{
      std::string environnement[] = 
      {
        "REQUEST_METHOD=" + req.gettype(),
        "QUERY_STRING=" + ((req.gettype().compare("GET") == 0) ? ParseUri(req.geturi())  : " "), // si c'est une get je mets rien apres a voir si on met une valeur ou pas
        "CONTENT_TYPE=" + _contentType, // content-type request
        "HTTP_HOST=" + req.gethostname(),
        "SCRIPT_NAME=" + server.getService(), // ici le nom du script je pense pas que ce soit bon
        "PATH_INFO=" + req.geturi(), // tout url 
    };
    int  environSize = sizeof(environnement) / sizeof(environnement[0]);
    char** envCGI = new char*[environSize + 1]; 
    for (int i = 0; i < environSize; i++) 
    {
        envCGI[i] = new char[environnement[i].size() + 1];
        strcpy(envCGI[i], environnement[i].c_str());
    }
    envCGI[environSize] = NULL;

    return envCGI;
}



std::string playCgi(const std::string &path, Request req, char **env) 
{
    int pipe_in[2];
    int pipe_out[2];
    pid_t pid;
    std::string newBody;

    if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) 
        return "Status: 500\r\n\r\n";
    pid = fork();
    if (pid == -1) 
    {
        closePipe(pipe_in, pipe_out);
        return "Status: 500\r\n\r\n";
    }
    if (pid == 0)
    {
        if (UtilParsing::recoverExtension(path) == ".pl")
            childProcessCgi(env, pipe_in, pipe_out);
        else
            childProcessCgiPy(env, pipe_in, pipe_out);
    }
    else 
        return parentProcessCgi(req, pid, pipe_in, pipe_out);

    return "Error";
}


void closePipe(int *pipe_in, int *pipe_out)
{
    close(pipe_in[0]); 
    close(pipe_in[1]);
    close(pipe_out[0]); 
    close(pipe_out[1]);
}

std::string executeCGI(const std::string &path, Server server, Request req)
{
    char **env;
    std::string body;
    if (controlContentBodyReq(req) == -1)
        throw ErrorCGI("No Content", 204);
    env = initEnv(req, server);
    body = playCgi(path, req, env);
    if (env)
        freeEnv(env);
    return body;
}

int controlContentBodyReq(Request req)
{
    if (req.gettype().compare("POST"))
    {
        if (req.getbody().empty())
            return -1;
    }
    return 0;
}

// rajouter le nom du script a appele de facon modulable
void childProcessCgi(char**env, int *pipe_in, int *pipe_out)
{
    close(pipe_in[1]);
    close(pipe_out[0]);
    dup2(pipe_in[0], STDIN_FILENO);
    close(pipe_in[0]);
    dup2(pipe_out[1], STDOUT_FILENO);
    close(pipe_out[1]);
    const char *args[] = {"/usr/bin/perl", "./cgi-bin/script.pl", NULL}; // ici
    execve(args[0], (char *const *)args, env);
    _exit(1); // ici gestion d'erreur 
}


// rajouter le nom du script a appele de facon modulable
void childProcessCgiPy(char**env, int *pipe_in, int *pipe_out)
{
    close(pipe_in[1]);
    close(pipe_out[0]);
    dup2(pipe_in[0], STDIN_FILENO);
    close(pipe_in[0]);
    dup2(pipe_out[1], STDOUT_FILENO);
    close(pipe_out[1]);
    const char *args[] = {"/usr/bin/python3", "./cgi-bin/script.py", NULL}; // ici
    execve(args[0], (char *const *)args, env);
    _exit(1); // ici gestion d'erreur 
}



std::string parentProcessCgi(Request req, pid_t pid, int *pipe_in, int *pipe_out)
{
    std::string newBody;

    close(pipe_in[0]);
    close(pipe_out[1]);
    write(pipe_in[1], req.getbody().c_str(), req.getbody().length()); // avant check si le body envoyer a un content sinon renvoyer error 204 No content
    close(pipe_in[1]);
    newBody = createBody(pipe_out);
    close(pipe_out[0]);
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        return "Status: 500\r\n\r\n";
    return newBody;
}

std::string createBody(int *pipe_out)
{
    std::string newBody;
    char buffer[4096];
    ssize_t bytes_read;   
    while ((bytes_read = read(pipe_out[0], buffer, sizeof(buffer) - 1)) > 0) 
    {
        buffer[bytes_read] = '\0';
        newBody.append(buffer);
    }
    return newBody;
}

std::string ParseUri(std::string uri)
{
    std::string::size_type start = uri.find('?');
    if (start == std::string::npos)
        return "";
    return UtilParsing::convertHexaToString(uri.substr(start + 1));
}

void freeEnv(char** tab)
{
    int i = 0;
    while (tab[i] != NULL)
    {
        delete[] tab[i];
        i++;
    }
    delete[] tab;
}

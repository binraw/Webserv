
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

#include "Client.hpp"
#include <cstring>

class ErrorCGI : virtual public std::exception 
{
    public:
        ErrorCGI(const std::string &message) : _message(message){}
        virtual const char *	what() const throw() { return _message.c_str(); }
    private:
        std::string _message;

};

// ici voir si il faut renvoyer une page error dans le catch
void checkFileScript(const std::string &path, Server server, Request req)
{
    std::string response;
    try 
    {
        if (UtilParsing::fileExits(path) != 0)
            throw ErrorCGI("");
        if (access(path.c_str(), X_OK) != 0)
            throw ErrorCGI("");
        if (checkExtensionCGI(path, server) != 0)
            throw ErrorCGI("");
        if (moveToDirectoryScript(extractDirectory(path)) != 0)
            throw ErrorCGI("");
        response = executeCGI(path, server, req); // ici plus tard renvoyer directement dans la value response
        if (response.empty())
            throw ErrorCGI("");
    }
    catch (const ErrorCGI& e)
    {

    }
}
// fonction pour check le type d'extension du script demander pour verifier si le server la supporte
// pareil la recuperer dans le parsing le nom par server  le nom du script cgi pour verifier son extension
bool checkExtensionCGI(const std::string &path, Server server)
{
   std::string cgi_path = server.getConfig()._CgiPath;

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
    {
        std::cerr << "Error move directory script." << std::endl;
        return false;
    }
    return true;
}

// ici recuperer les values de l'env avec les valeurs de la request/server
// ne pas oublier de delete tout le tab
std::string _method,  _params, _contentType, _http, _httpReferer, _remoteAddr, _remotePort, _scriptName, _pathInfo;
char** initEnv(Request req, Server server)
{
      std::string environnement[] = {
        "REQUEST_METHOD=" + req.gettype(),
        "QUERY_STRING=" + req.getbody(),
        "CONTENT_TYPE=" + _contentType,
        "HTTP_HOST=" + req.gethostname(),
        "SCRIPT_NAME=" + server.getService(),
        "PATH_INFO=" + _pathInfo, // 
    };
    int  environSize = sizeof(environnement) / sizeof(environnement[0]);
    char** environ = new char*[environSize + 1]; 

    for (int i = 0; i < environSize; i++) 
    {
        environ[i] = new char[environnement[i].size() + 1];
        strcpy(environ[i], environnement[i].c_str());
    }
    environ[environSize] = NULL;

    return environ;
}


// voila pourquoi pour moi dans le sujet il y a ecrit :
// "votre serveur devrait fonctionner avec un seul CGI"
// car on donne le chemin de executable en dur et si on veut rajouter des langage soutenu 
// alors il faut faire differente route lors de l'execution
std::string playCGI(const std::string path, char** env)
{
    std::string output;
    int pipfd[2];

    if (pipe(pipfd) == -1)
        return "";
    pid_t pid = fork();
    if (pid < 0)
    {
        close(pipfd[0]);
        close(pipfd[1]);
        return "";
    }
    else if (pid == 0) 
    {
        if (childProcess(path, env, pipfd) == 1)
            exit(1);
        exit(0);
    } 
    else 
        output = parentProcess(pid, pipfd);

    return output;
}

int childProcess(const std::string path, char**env, int *pipfd)
{
    close(pipfd[0]);
    dup2(pipfd[1], STDOUT_FILENO); 
    close(pipfd[1]);
    char *args[] = {"/usr/bin/perl", (char*)(path.c_str()), NULL};
    if (execve(args[0],args , env) == -1)
        return 1;
    return 0;
}

std::string parentProcess(pid_t pid, int *pipfd)
{
    std::string output;
    char buffer[128];
    ssize_t bytesRead;
    close(pipfd[1]);
    while ((bytesRead = read(pipfd[0], buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytesRead] = '\0';
        output.append(buffer);
    }
    close(pipfd[0]);
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        return output;
    else 
        return "";
}

std::string executeCGI(const std::string &path, Server server, Request req)
{
    char **env;
    std::string body;
    env = initEnv(req, server);
    body = playCGI(path, env);
    return body;
}
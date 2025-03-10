
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
#include "CGI.hpp"

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


// ici voir si il faut renvoyer une page error dans le catch
// void processCGI(const std::string &path, Server server, Request req)
// {
//     std::string response;
//     try 
//     {
//         if (UtilParsing::fileExits(path) != 0)
//             throw ErrorCGI("Not found", 404);
//         if (access(path.c_str(), X_OK) != 0)
//             throw ErrorCGI("Not found", 404);
//         if (checkExtensionCGI(path, server) != 0)
//             throw ErrorCGI("Bad Gateway", 502);
//         if (moveToDirectoryScript(extractDirectory(path)) != 0)
//             throw ErrorCGI("Internal server error", 500);
//         response = executeCGI(path, server, req); // ici plus tard renvoyer directement dans la value response
//         if (response.empty())
//             throw ErrorCGI("Bad Gateway", 502);
//     }
//     catch (const ErrorCGI& e)
//     {
//          std::cerr << e.what() << std::endl;
//     }
// }
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

// changement tout les args seront dans le body dans tout les cas
// meme dans le cas d'une requet get
// donc juste a faire une convert hexa sur les args si c'est une get
std::string _method,  _params, _contentType, _http, _httpReferer, _remoteAddr, _remotePort, _scriptName, _pathInfo;
// char** initEnv(Request req, Server server)
// {
//       std::string environnement[] = {
//         "REQUEST_METHOD=" + req.gettype(),
//         "QUERY_STRING=" + (req.gettype().compare("GET") == 0) ? ParseUri(req.geturi())  : req.getbody(),
//         "CONTENT_TYPE=" + _contentType, // content-type request
//         "HTTP_HOST=" + req.gethostname(),
//         "SCRIPT_NAME=" + server.getService(),
//         "PATH_INFO=" + _pathInfo, // tout url 
//     };
//     int  environSize = sizeof(environnement) / sizeof(environnement[0]);
//     char** environ = new char*[environSize + 1]; 

//     for (int i = 0; i < environSize; i++) 
//     {
//         environ[i] = new char[environnement[i].size() + 1];
//         strcpy(environ[i], environnement[i].c_str());
//     }
//     environ[environSize] = NULL;

//     return environ;
// }


// route pour effectuer le pipe + fork neccessaire pour l'execution 
// si erreur on renvoie juste un string vide car le controle se fait plus haut
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
        childProcess(path, env, pipfd);

    else 
        output = parentProcess(pid, pipfd);

    return output;
}


// je dois enfaite creer un fichier tempo
// qui va avoir tout lee body de la request POST

int childProcessPostTEST(const std::string path, char**env, int *pipfd)
{
    const char *filename;
    std::FILE *tempfile = std::tmpfile();
    long fdIn = fileno(tempfile);
    std::string body = "{\"name\": \" John Doe\"}";
    write(fdIn, body.c_str(), body.size());
    lseek(fdIn, 0, SEEK_SET);
    close(fdIn); // Fermer fdIn après avoir écrit les données

    dup2(pipfd[0], STDIN_FILENO); // Dupliquer pipfd[0] sur STDIN_FILENO
    close(pipfd[0]); // Fermer pipfd[0]
    dup2(pipfd[1], STDOUT_FILENO); // Dupliquer pipfd[1] sur STDOUT_FILENO
    close(pipfd[1]); // Fermer pipfd[1]

    std::string::size_type start = path.find_last_of("/");
    if (start != std::string::npos)
        filename = path.substr(start + 1).c_str();
    else
        filename = path.c_str();
    const char *args[] = {"/usr/bin/perl", filename, NULL};
    if (execve(args[0],(char* const*)args , env) == -1)
        return 1;
    return 0;
}


// ici avec les arg dans query_string
int childProcess(const std::string path, char**env, int *pipfd)
{
    const char *filename;

    close(pipfd[0]);
    dup2(pipfd[1], STDOUT_FILENO); 
    close(pipfd[1]);
    std::string::size_type start = path.find_last_of("/");
    if (start != std::string::npos)
        filename = path.substr(start + 1).c_str();
    else
        filename = path.c_str();
    const char *args[] = {"/usr/bin/perl", filename, NULL};
    if (execve(args[0],(char* const*)args , env) == -1)
        return 1;
    return 0;
}
// le process parent qui va ecrire dans output le retour du script qui dans l'idee 
// s'occupe de creer un body html
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

// std::string executeCGI(const std::string &path, Server server, Request req)
// {
//     char **env;
//     std::string body;
//     env = initEnv(req, server);
//     body = playCGI(path, env);
//     return body;
// }

// -------------------------ICI COMMENCE LA PARTIE TEST----------------------------


void processCGITEST(const std::string &path)
{
    std::string response;
    try 
    {
        if (UtilParsing::fileExits(path) != true) //ici voir mais a enlever le check normalement
            throw ErrorCGI("Not found", 404);
        if (access(path.c_str(), X_OK) != 0)
            throw ErrorCGI("Not found", 404);
        if (checkExtensionCGITEST(path) != true)
            throw ErrorCGI("Bad Gateway", 502);
        if (moveToDirectoryScript(extractDirectory(path)) != true)
            throw ErrorCGI("Internal server error", 500);
        response = executeCGITEST(path); 
        if (response.empty())
            throw ErrorCGI("Bad Gateway", 502);
    }
    catch (const ErrorCGI& e)
    {
        std::cerr << e.what() << std::endl;
    }
    std::cout << response << std::endl;
}


// ici on pourra rajouter d'autres option si on veut faire fonctionner d'autre type de CGI
bool checkExtensionCGITEST(const std::string &path)
{
   std::string cgi_path = "support.pl";

    if (UtilParsing::recoverExtension(path) == UtilParsing::recoverExtension(cgi_path))
        return true;
    return false;
}

std::string executeCGITEST(const std::string &path)
{
    char **env;
    std::string body;
    env = initEnvTEST();
    body = playCGITEST(path, env);
    freeEnv(env);
    return body;
}

char** initEnvTEST()
{
      std::string environnement[] = {
        "REQUEST_METHOD=POST",
        "QUERY_STRING=value1=35",
        "CONTENT_TYPE=text/html",
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


std::string ParseUri(std::string uri)
{
    std::string::size_type start = uri.find('?');
    if (start == std::string::npos)
        return "";
    return UtilParsing::convertHexaToString(uri.substr(start + 1));
}

std::string playCGITEST(const std::string path, char** env)
{
    std::string output;
    int pipfd[2];
    int			saveStdin;
	int			saveStdout;
    saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);

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
        childProcessPostTEST(path, env, pipfd);
    else 
        output = parentProcess(pid, pipfd);
    dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
    close(saveStdin);
	close(saveStdout);

    return output;
}

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


// std::string executeCgi(char **env) {
//     int pipefd[2];
//     pid_t pid;
//     std::string newBody;
//     std::string body = "objectif=Creation+de+site+web+pour+entreprise\\&design=oui\\&rdv=non\\&delai=2+mois\\&maintenance=oui\\&SEO=non";
//     // Créer un pipe
//     if (pipe(pipefd) == -1) {
//         return ("Status: 500\r\n\r\n");
//     }

//     pid = fork();

//     if (pid == -1) {
//         return ("Status: 500\r\n\r\n");
//     } else if (!pid) {

//         close(pipefd[1]);


//         dup2(pipefd[0], STDIN_FILENO);

//         const char *args[] = {"./cgi-bin/script.pl", NULL};
//         execve("/usr/bin/perl", (char *const *)args, env);


//         write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
//         exit(0);
//     } else {

//         close(pipefd[0]);
//         // std::cout << body << std::endl;
//         write(pipefd[1], body.c_str(), body.size());


//         close(pipefd[1]);


//         waitpid(pid, NULL, 0);

//         char buffer[1024];
//         int ret;
//         while ((ret = read(pipefd[0], buffer, 1024)) > 0) 
//         {
//             newBody += std::string(buffer, ret);
//         }
//         close(pipefd[0]);
//     }

//     return newBody;
// }


std::string executeCgi(char **env) 
{
    int pipefd[2];
    pid_t pid;
    std::string newBody;
    std::string body = "objectif=Creation+de+site+web+pour+entreprisedesign=ouirdv=nondelai=2+moismaintenance=ouiSEO=non";


if (pipe(pipefd) == -1) 
{
    return ("Status: 500\r\n\r\n");
}

pid = fork();

if (pid == -1) 
{
    return ("Status: 500\r\n\r\n");
} 
else if (!pid) 
{

    close(pipefd[1]);

    dup2(pipefd[0], STDIN_FILENO); 

    close(pipefd[0]); 

    const char *args[] = {"./cgi-bin/script.pl", NULL};
    execve("/usr/bin/perl", (char *const *)args, env);

    write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
    exit(0);
} 
else 
{
    close(pipefd[0]); 

    write(pipefd[1], body.c_str(), body.size());

    close(pipefd[1]); 


    waitpid(pid, NULL, 0);

    char buffer[1024];
    int ret;
    std::cout << "rentre" << std::endl;
    while ((ret = read(pipefd[0], buffer, 1024)) > 0) 
    {
        std::cout << "valeur de buffer : " << buffer << std::endl;
        newBody += std::string(buffer, ret);
    }
    close(pipefd[0]);
}

    return newBody;
}



// std::string executeCgi(char **env) 
// {
//     int pipe_in[2], pipe_out[2];
//     pid_t pid;
//     std::string newBody;

//     // Données POST bien formatées
//     std::string body = "objectif=Creation+de+site+web+pour+entreprise";

//     // Créer deux pipes : un pour envoyer les données, un pour lire les résultats
//     if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
//         return "Status: 500\r\n\r\n";
//     }

//     if ((pid = fork()) == -1) {
//         return "Status: 500\r\n\r\n";
//     }

//     if (pid == 0) { // Processus enfant
//         close(pipe_in[1]); // Fermer l'extrémité écriture de pipe_in
//         close(pipe_out[0]); // Fermer l'extrémité lecture de pipe_out

//         dup2(pipe_in[0], STDIN_FILENO);  // Rediriger STDIN
//         dup2(pipe_out[1], STDOUT_FILENO); // Rediriger STDOUT

//         close(pipe_in[0]);
//         close(pipe_out[1]);

//         const char *args[] = {"/usr/bin/perl", "./cgi-bin/script.pl", NULL};
//         execve(args[0], (char *const *)args, env);

//         // Si execve échoue
//         _exit(1);
//     } 
//     else 
//     { 
//         close(pipe_in[0]); // Fermer l'extrémité lecture de pipe_in
//         close(pipe_out[1]); // Fermer l'extrémité écriture de pipe_out

//         // Envoyer les données POST
//         write(pipe_in[1], body.c_str(), body.size());
//         close(pipe_in[1]); // Fin de l'envoi

//         // Lire la réponse CGI
//         char buffer[1024];
//         int ret;
//         while ((ret = read(pipe_out[0], buffer, sizeof(buffer))) > 0) {
//             newBody.append(buffer, ret);
//         }
//         close(pipe_out[0]);

//         // Attendre la fin du processus enfant
//         waitpid(pid, NULL, 0);
//     }
//     std::cout << "valeur de new body :" << newBody << std::endl;
//     return newBody;
// }



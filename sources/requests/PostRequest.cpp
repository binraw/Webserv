




#include "PostRequest.hpp"

PostRequest::PostRequest(const std::string &) : ARequest()
{

}

PostRequest::PostRequest(const PostRequest &) : ARequest()
{

}

PostRequest::~PostRequest()
{

}

PostRequest &PostRequest::operator=(const PostRequest &)
{
	return *this;
}

std::ostream & operator<<(std::ostream &o, const PostRequest&)
{
	return o;
}


void PostRequest::executePostRequest()
{
	if (_url.find("POST /cgi-bin"))
	{
		if (fileExits(_pathCGI))
		{
			_contentFileUpdate = playCGI();
			return; // ici je return car on attend juste que contentFileUpload soit rempli.
		}
		else
			throw;
	}
	else if (_url.find("POST" + _config._uploadPath))
	{
		if (directoryExists(_config._uploadPath))
		{
			// ici refaire la fonction pour telecharger sans cgi.
		}
	}
}



bool fileExits(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	return file.good();
}

bool directoryExists(const std::string &dirname)
{
	struct stat info;
	if (stat(dirname.c_str(), &info) != 0)
		return false;
	return (info.st_mode & S_IFDIR) != 0;
}

std::string PostRequest::playCGI()
{
    std::string output;
    int pipfd[2];

    if (pipe(pipfd) == -1)
    {
        std::cerr << "Error pipe CGI" << std::endl;
        return "";
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "fork failled" << std::endl;
        return "";
    }
    else if (pid == 0) 
    {
        close(pipfd[0]);
        dup2(pipfd[1], STDOUT_FILENO); 
        close(pipfd[1]);
        char* const args[] = {const_cast<char*>(_pathCGI.c_str()), NULL};
        if (execv(args[0], args) == -1)  
        {
            std::cerr << "execv failed" << std::endl;
            return "";
        }
    } 
    else 
    {
        close(pipfd[1]);
        char buffer[128];
        ssize_t bytesRead;
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
        {
            std::cout << "CGI script did not terminate normally" << std::endl;
        }
    }
    return output;
}


// fct a changer vu qu'il n'y a plus de CGI dassn le telechargement.
std::string PostRequest::playUploadCGI()
{
     std::string output;
    int pipfd[2];
    
    if (pipe(pipfd) == -1)
    {
        std::cerr << "Error pipe CGI" << std::endl;
        return "";
    }
    
    pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "fork failed" << std::endl;
        return "";
    }
    else if (pid == 0) 
    {
        close(pipfd[0]);
        dup2(pipfd[1], STDOUT_FILENO); 
        close(pipfd[1]);
        char* const args[] = {const_cast<char*>(_pathCGI.c_str()), NULL};
        write(STDIN_FILENO, _contentFileForUpload.c_str(), _contentFileForUpload.size());
        if (execv(args[0], args) == -1)  
        {
            std::cerr << "execv failed" << std::endl;
            return "";
        }
    } 
    else 
    {
        close(pipfd[1]);
        char buffer[128];
        ssize_t bytesRead;
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
        {
            std::cout << "CGI script did not terminate normally" << std::endl;
        }
    }
    return output;
}
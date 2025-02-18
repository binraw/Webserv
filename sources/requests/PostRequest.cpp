




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

int PostRequest::executePostRequest()
{
	if (_url.find("POST /cgi-bin"))
	{
		if (UtilParsing::fileExits(_pathCGI))
		{
			_contentFileUpdate = playCGI();
            if (_contentFileForUpload.empty())
                return 500; // la on met une Internal Server Error car une erreur c'est produite dans l'execution
			return 200; // request Reussi
		}
		else
			throw;
	}
	else if (_url.find("POST" + _config._uploadPath))
	{
		if (UtilParsing::directoryExists(_config._uploadPath))
		{
			return save_file(_contentReq);
		}
	}
    else
    {
       return 400; // la on retourne bad request car notre server ne supprote que ces deux type de POST. 
    }
}


std::string PostRequest::playCGI()
{
    std::string output;
    int pipfd[2];

    if (pipe(pipfd) == -1)
    {
        std::cerr << "Error pipe CGI" << std::endl; // juste pour futur debug
        return "";
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "fork failled" << std::endl; // juste pour futur debug
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
            std::cerr << "execv failed" << std::endl; // juste pour futur debug
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


std::string PostRequest::extract_filename(const std::string& request_body) // a voir si cette fonction peut etre utile ailleurs. Donc pas que la mettre dans POST
{
    std::string filename;
    std::string key = "filename=\"";

    size_t pos = request_body.find(key);
    if (pos != std::string::npos) 
    {
        pos += key.length();
        size_t end_pos = request_body.find("\"", pos);
        if (end_pos != std::string::npos) {
            filename = request_body.substr(pos, end_pos - pos);
        }
    }
    if (filename.find("..") != std::string::npos) // rajout car si on a .. en nom de fichier ca peut faire de la merde et le telecharger ailleurs
    {
        return "";
    }
    return filename;
}

int PostRequest::save_file(const std::string& request_body) 
{
    std::string filename = extract_filename(request_body);
    if (filename.empty()) 
    {
        return 404;
    }
    std::string boundary = "------WebKitFormBoundary";
    size_t file_start = request_body.find("\r\n\r\n");
    if (file_start == std::string::npos) 
    {
        return 404;
    }
    file_start += 4;
    size_t file_end = request_body.rfind("--" + boundary);
    if (file_end == std::string::npos) 
    { 
        return 413; // taille max (fichier trop gros)
    }
    std::string file_data = request_body.substr(file_start, file_end - file_start);
    std::string file_path = "upload/" + filename;
    std::ofstream output(file_path.c_str(), std::ios::binary); // le ios binary permet de pas alterer le contenu donc gerer meme les images etc 
    if (!output) 
    {
        return 500; // la erreur au niveau du server on donne pas plus d'info qu'une erreur 500 
    }
    output.write(file_data.c_str(), file_data.size());
    output.close();
    return 200; // tout est ok
}

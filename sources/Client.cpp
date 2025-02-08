#include "Client.hpp"

// Client::Client(int fd, std::string request)
// {
// }

Client::Client(std::string request)
{
    _pathCGI = request;
}
Client::~Client()
{
}

std::string Client::playCGI()
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
            return "";
    } 
    else 
    {
        close(pipfd[1]);
        char buffer[128];
        while (ssize_t bytesRead = read(pipfd[0], buffer, sizeof(buffer) - 1) > 0)
        {
            buffer[bytesRead] = '\0';
            output.append(buffer);
        }
        close(pipfd[0]);
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) 
        {
            std::cout << "'" << output << "'" << std::endl;
            std::cout << "CGI script exited with status: " << WEXITSTATUS(status) << std::endl;
        } else 
        {
            std::cout << "CGI script did not terminate normally" << std::endl;
        }
    }
    return output;
}

// #include "ARequest.hpp"


// ARequest::~ARequest()
// {
// }

// ARequest* handleRequest(std::string request)
// {
//     // char buffer[1024];
//     // int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
//     // buffer[bytesRead] = '\0';
//     // std::string request = std::string(buffer);

//     if (request.find("GET /cgi-bin/") != std::string::npos)
//     {
//         //ici extraire le chemin et traiter la requete CGi
//     }
//     else if (request.find("POST /cgi-bin/") != std::string::npos)
//     {
//         //ici traiter une post avec cgi
//     }
//     else if (request.find("DELETE") != std::string::npos)
//     {

//     }
//     else if (request.find("GET") != std::string::npos)
//     {

//     }
//     else if (request.find("POST") != std::string::npos)
//     {

//     }
//     else
//     {
//         std::cerr << "Bad request" << std::endl;
//     }
//     return NULL;
// }

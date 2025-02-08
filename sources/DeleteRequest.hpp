#ifndef DELETEREQUEST_HPP
# define DELETEREQUEST_HPP

#include "ARequest.hpp"

class DeleteRequest : public ARequest
{
private:

public:
    void handle() override; // ici gestion pour la requet

};




#endif
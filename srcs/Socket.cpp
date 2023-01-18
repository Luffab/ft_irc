#include "Socket.hpp"

Socket::Socket(User* user, int id): _user(user), _id(id)
{
}

Socket::~Socket() {}

User* Socket::getUser()
{
    return (_user);
}

int	Socket::getID()
{
	return (_id);
}
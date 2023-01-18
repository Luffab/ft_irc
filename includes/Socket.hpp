#pragma once

#include "User.hpp"

class	User;

class Socket
{
	private:
		User*	_user;
		int		_id;

	public:
		Socket(User* user, int id);
		~Socket();

		User* getUser();
		int getID();
};
#include "User.hpp"

User::User(int id): _is_authentified(false), _is_modo(false), _is_registered(false), _id(id)
{
	_nick = "";
}

User::~User() {}

void    User::setAuth(bool auth)
{
    this->_is_authentified = auth;
}

void    User::setNick(std::string str)
{
    _nick = str;
}

void    User::setUsername(std::string str)
{
    _username = str;
}

void    User::setHostname(std::string str)
{
    _hostname = str;
}

void    User::setServname(std::string str)
{
    _servname = str;
}

void    User::setRealname(std::string str)
{
    _realname = str;
}

void    User::setRegistered(bool registered)
{
    this->_is_registered = registered;
}

void	User::setModo(bool is_modo)
{
	_is_modo = is_modo;
}

bool User::getAuth()
{
    return (this->_is_authentified);
}

std::string User::getNick()
{
    return (_nick);
}

std::string User::getUsername()
{
    return (_username);
}

std::string User::getHostname()
{
    return (_hostname);
}

bool User::getRegistered()
{
    return (this->_is_registered);
}

int User::getId()
{
    return (_id);
}

int	User::getModo()
{
	return (_is_modo);
}

std::vector<Channel> &User::getBanChannels()
{
	return (_bann_channels);
}

void	User::pushBackBannChannels(Channel &channel)
{
	_bann_channels.push_back(channel);
}

std::vector<Channel>::iterator	User::searchChannel(Channel& channel, User& user)
{
	std::vector<Channel>::iterator	it = user._bann_channels.begin();
	unsigned long int			i = 0;
	
	while (i < user._bann_channels.size())
	{
		if (user._bann_channels[i].getName() == channel.getName())
			return (it);
		i++;
		it++;
	}
	return (it);
}

void	User::removeBannChannel(Channel& channel)
{
	std::vector<Channel>::iterator	it = searchChannel(channel, *this);
	_bann_channels.erase(it);
}
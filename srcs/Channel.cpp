/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpuccion <dpuccion@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 03:41:58 by fatilly           #+#    #+#             */
/*   Updated: 2022/08/23 21:16:11 by dpuccion         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// ! CONSTRUCTEUR ET DESTRUCTEUR ! //

Channel::Channel(std::string name, Server* server, User& user): _name(name), _topic(""), _password(""), _max_users(0), _server(server)
{
	_is_moderated = false;
	_is_invite_only = false;
	_has_password = false;
	_has_topic_protection = false;
	_is_max_users = false;
	_has_no_ext_msg = false;
	_users.push_back(&user);
	_operators.push_back(&user);
	addModoInOperator();
	addModoInVoiced();
}

Channel::~Channel() {}

// ! LEAVE AND JOIN ! //

void	Channel::joinChannel(User& user)
{
	unsigned long int			i = 0;
	while (i < _users.size())
	{
		if (_users[i]->getId() == user.getId())
		{
			sendChannelErrorMessage(user, ALREADYINCHAN, user.getNick(), "You are already in channel");
			return ;
		}
		i++;
	}
	if (_is_max_users == true)
	{
		if (_users.size() == _max_users)
			sendChannelErrorMessage(user, ISCHANFULL, user.getNick(), "This channel is full");
		else
		{
			_users.push_back(&user);
			sendChannelMessage(user, "JOIN", _name, user.getId());
		}
	}
	else
	{
		_users.push_back(&user);
		sendChannelMessage(user, "JOIN", _name, user.getId());
	}

}

void	Channel::leaveChannel(User& user)
{
	std::vector<User*>::iterator	it;
	unsigned long int			i = 0;

	it = _users.begin();
	while (i < _users.size())
	{
		if (_users[i]->getId() == user.getId())
		{
			_users.erase(it);
			return ;
		}
		i++;
		it++;
	}
	sendChannelErrorMessage(user, ALREADYINCHAN, user.getNick(), "You are not in this channel");
}

void	Channel::kickUser(User& op_user, User& bann_user, std::string explaination)
{
	std::vector<User*>::iterator	it;

	if (isUserExist(op_user, _operators))
	{
		if (!isUserExist(bann_user, _operators) || op_user.getModo() == true)
		{	
			if (isUserExist(bann_user, _users))
			{
				it = searchUser(bann_user, _users);
				//std::string msg = ":" + op_user.getNick() + " KICK #" + _name + " " + bann_user.getNick() + "\n";
				std::string msg = ":" + op_user.getNick() + "!" + op_user.getUsername() + "@" + op_user.getHostname() + " PRIVMSG " + bann_user.getNick() + " :" + "You have been kicked from #" + _name + " for reason : " + explaination + "\n";
				send(bann_user.getId(), msg.c_str(), msg.size(), 0);
				_users.erase(it);
			}
			else
				sendChannelErrorMessage(op_user, ALREADYINCHAN, op_user.getNick(), "The user is not in this channel");
		}
		else
			sendChannelErrorMessage(op_user, ALREADYOP, op_user.getNick(), "The user is an operator");
	}
	else
		sendChannelErrorMessage(op_user, NOACCESS, op_user.getNick(), "You have no rights");
}

// ! CODAGE DES MODES ! //

void	Channel::addTopic(std::string topic, User& user)
{
	if (_has_topic_protection == true)
	{
		if (isUserExist(user, _operators))
			_topic = topic;
		else
			sendChannelErrorMessage(user, NOACCESS, user.getNick(), "You have no rights");
	}
	else
		_topic = topic;
}

void	Channel::removeTopic(User& user)
{
	if (_has_topic_protection == true)
	{
		if (isUserExist(user, _operators))
			_topic = "";
		else
			sendChannelErrorMessage(user, NOACCESS, user.getNick(), "You have no rights");
	}
	else
		_topic = "";
}

void	Channel::addTopicProtection(User& op_user)
{
	if (isUserExist(op_user, _operators))
		_has_topic_protection = true;
	else
		sendChannelErrorMessage(op_user, NOACCESS, op_user.getNick(), "You have no rights");
}

void	Channel::removeTopicProtection(User& op_user)
{
	if (isUserExist(op_user, _operators))
		_has_topic_protection = false;
	else
		sendChannelErrorMessage(op_user, NOACCESS, op_user.getNick(), "You have no rights");
}

void	Channel::bannUser(User& op_user, User& bann_user)
{
	std::vector<User*>::iterator	it;

	if (isUserExist(op_user, _operators))
	{
		if (!isUserExist(bann_user, _operators) || op_user.getModo() == true)
		{	
			if (isUserExist(bann_user, _users))
			{
				it = searchUser(bann_user, _users);
				_banned_users.push_back(&bann_user);
				bann_user.pushBackBannChannels(*this);
				_users.erase(it);
			}
			else
				sendChannelErrorMessage(op_user, ALREADYINCHAN, op_user.getNick(), "The user is not in this channel");
		}
		else
			sendChannelErrorMessage(op_user, ALREADYOP, op_user.getNick(), "The user is an operator");
	}
	else
		sendChannelErrorMessage(op_user, NOACCESS, op_user.getNick(), "You have no rights");
}

void	Channel::debannUser(User& op_user, User& debann_user)
{
	std::vector<User*>::iterator	it;

	if (isUserExist(op_user, _operators))
	{
		if (isUserExist(debann_user, _banned_users))
		{
			it = searchUser(debann_user, _banned_users);
			_banned_users.erase(it);
			debann_user.removeBannChannel(*this);
			_users.push_back(&debann_user);
		}
		else if (isUserExist(debann_user, _users))
			sendChannelErrorMessage(op_user, ALREADYBAN, op_user.getNick(), "The user is not ban");
		else
			sendChannelErrorMessage(op_user, ALREADYINCHAN, op_user.getNick(), "The user is not in this channel");
	}
	else
		sendChannelErrorMessage(op_user, NOACCESS, op_user.getNick(), "You have no rights");
}

void	Channel::inviteOnly(User& user)
{
	if (isUserExist(user, _operators))
		_is_invite_only = true;
	else
		sendChannelErrorMessage(user, NOACCESS, user.getNick(), "You have no rights");
}

void	Channel::removeInviteOnly(User& user)
{
	if (isUserExist(user, _operators))
		_is_invite_only = false;
	else
		sendChannelErrorMessage(user, NOACCESS, user.getNick(), "You have no rights");
}

void	Channel::addKeyProtect(User& user, std::string password)
{
	if (isUserExist(user, _operators))
	{
		_has_password = true;
		_password = password;
	}
	else
		sendChannelErrorMessage(user, NOACCESS, user.getNick(), "You have no rights");
}

void	Channel::removeKeyProtect(User& user)
{
	if (isUserExist(user, _operators))
	{
		_has_password = false;
		_password = "";
	}
	else
		sendChannelErrorMessage(user, NOACCESS, user.getNick(), "You have no rights");
}

void	Channel::addLimitUsers(User& user, unsigned long int nb)
{
	if (nb >= _users.size())
	{
		if (isUserExist(user, _operators))
		{
			_is_max_users = true;
			_max_users = nb;
		}
		else
			sendChannelErrorMessage(user, NOACCESS, user.getNick(), "You have no rights");
	}
	else
		sendChannelErrorMessage(user, NOACCESS, user.getNick(), "Too many user in the channel");
}

void	Channel::removeLimitUsers(User& user)
{
	if (isUserExist(user, _operators))
	{
		_is_max_users = false;
		_max_users = 0;
	}
	else
		sendChannelErrorMessage(user, NOACCESS, user.getNick(), "You have no rights");
}

void	Channel::addModerated(User& user)
{
	if (isUserExist(user, _operators))
		_is_moderated = true;
	else
		sendChannelErrorMessage(user, NOACCESS, user.getNick(), "You have no rights");
}

void	Channel::removeModerated(User& user)
{
	if (isUserExist(user, _operators))
		_is_moderated = false;
	else
		sendChannelErrorMessage(user, NOACCESS, user.getNick(), "You have no rights");
}

void	Channel::addNoExternalMessage(User& user)
{
	if (isUserExist(user, _operators))
		_has_no_ext_msg = true;
	else
		sendChannelErrorMessage(user, NOACCESS, user.getNick(), "You have no rights");
}

void	Channel::removeNoExternalMessage(User& user)
{
	if (isUserExist(user, _operators))
		_has_no_ext_msg = false;
	else
		sendChannelErrorMessage(user, NOACCESS, user.getNick(), "You have no rights");
}

void	Channel::addOperator(User& op_user, User& user)
{
	if (isUserExist(op_user, _operators))
	{
		if (!isUserExist(user, _operators))
			_operators.push_back(&user);
		else
			sendChannelErrorMessage(op_user, ALREADYOP, op_user.getNick(), "The user is already an operator");
	}
	else
		sendChannelErrorMessage(op_user, NOACCESS, op_user.getNick(), "You have no rights");
}

void	Channel::removeOperator(User& op_user, User& user)
{
	std::vector<User*>::iterator	it;

	if (op_user.getModo() == true)
	{
		if (isUserExist(user, _operators))
		{
			it = searchUser(user, _operators);
			_operators.erase(it);
		}
		else
			sendChannelErrorMessage(op_user, ALREADYOP, op_user.getNick(), "The user is not an operator");
	}
	else
		sendChannelErrorMessage(op_user, NOACCESS, op_user.getNick(), "You have no rights");
}

void	Channel::addVoicedUser(User& op_user, User& user)
{
	if (isUserExist(op_user, _operators))
	{
		if (!isUserExist(user, _voiced))
			_voiced.push_back(&user);
		else
			sendChannelErrorMessage(op_user, HAVEVOICE, op_user.getNick(), "The user have already a voice");
	}
	else
		sendChannelErrorMessage(op_user, NOACCESS, op_user.getNick(), "You have no rights");
}

void	Channel::removeVoicedUser(User& op_user, User& user)
{
	std::vector<User*>::iterator	it;

	if (isUserExist(op_user, _operators))
	{
		if (isUserExist(user, _voiced) && isUserExist(user, _operators))
			sendChannelErrorMessage(op_user, ALREADYOP, op_user.getNick(), "The user is an operator");
		else if (!isUserExist(user, _voiced))
			sendChannelErrorMessage(op_user, HAVEVOICE, op_user.getNick(), "The user have no voice");
		else if (isUserExist(user, _voiced) && !isUserExist(user, _operators))
		{
			it = searchUser(user, _voiced);
			_voiced.erase(it);
		}
	}
	else
		sendChannelErrorMessage(op_user, NOACCESS, op_user.getNick(), "You have no rights");
}

// ! COMMANDES CHANNEL ! //

void	Channel::inviteUser(User& op_user, User& user)
{
	if (isUserExist(op_user, _operators))
	{
		if (!isUserExist(user, _users))
			_users.push_back(&user);
		else
			sendChannelErrorMessage(op_user, ALREADYINCHAN, op_user.getNick(), "The user is already in this channel");
	}
	else
		sendChannelErrorMessage(op_user, NOACCESS, op_user.getNick(), "You have no rights");
}

// ! FONCTIONS UTILES ! //

void	Channel::deleteUserById(int id, std::vector<User*>& vector)
{
	unsigned long int			j = 0;
	std::vector<User*>::iterator	it;

	it = vector.begin();
	while (j < vector.size())
	{
		if (id == vector[j]->getId())
		{
			vector.erase(it);
			return ;
		}
		j++;
		it++;
	}
}

std::vector<User*>::iterator	Channel::searchUser(User& user, std::vector<User*> &vector)
{
	std::vector<User*>::iterator	it;
	unsigned long int			i = 0;

	it = vector.begin();
	while (i < vector.size())
	{
		if (vector[i]->getId() == user.getId())
			return (it);
		i++;
		it++;
	}
	return (it);
}

int	Channel::isUserExist(User& user, std::vector<User*> vector)
{
	unsigned long int	i = 0;

	while (i < vector.size())
	{
		if (vector[i]->getId() == user.getId())
			return (1);
		i++;
	}
	return (0);
}

void	Channel::addModoInOperator()
{
	std::vector<User*>			operators_users;
	unsigned long int			i = 0;

	operators_users = _server->getOperatorsUsers();
	while (i < operators_users.size())
	{
		if (operators_users[i]->getModo() == 1)
			_operators.push_back(operators_users[i]);
		i++;
	}
}

void	Channel::addModoInVoiced()
{
	unsigned long int	i = 0;

	while (i < _operators.size())
	{
		_voiced.push_back(_operators[i]);
		i++;
	}
}

void	Channel::pushBackModoVector(User& user)
{
	_operators.push_back(&user);
}

void	Channel::pushBackVoicedVector(User& user)
{
	_voiced.push_back(&user);
}

// ! FONCTIONS GET ! //

std::string	Channel::getName()
{
	return (_name);
}

std::string	Channel::getTopic()
{
	return (_topic);
}

std::string	Channel::getPassword()
{
	return (_password);
}

int	Channel::getIfUserExist(User& user, std::vector<User*> vector)
{
	return (isUserExist(user, vector));
}

std::vector<User*>&	Channel::getOperatorsVector()
{
	return (_operators);
}

std::vector<User*>&	Channel::getUsersVector()
{
	return (_users);
}

std::vector<User*>&	Channel::getBannUsersVector()
{
	return (_banned_users);
}

std::vector<User*>&	Channel::getVoicedVector()
{
	return (_voiced);
}

bool	Channel::getIsInviteOnly()
{
	return (_is_invite_only);
}

bool	Channel::getHasPassword()
{
	return (_has_password);
}

bool	Channel::getHasNoExtMsg()
{
	return (_has_no_ext_msg);
}

bool	Channel::getIsModerated()
{
	return (_is_moderated);
}

void	sendChannelErrorMessage(User& user, std::string code, std::string dest, std::string message)
{
	std::string temp = ":IRC " + code + " " + dest + " :" + message + "\n";
	if (send(user.getId(), temp.c_str(), temp.size(), 0) == -1)
		std::cerr << "Send error" << std::endl;
}

void	sendChannelMessage(User& user, std::string message, std::string chanName, int j)
{
	std::string	temp = message + " #" + chanName + "\n";
	std::string temp2 = ":" + user.getNick() + " " + temp;
	if (send(j, temp2.c_str(), temp2.size(), 0) == -1)
		std::cerr << "Send error" << std::endl;
}

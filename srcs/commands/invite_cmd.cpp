/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite_cmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatilly <fatilly@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 05:00:55 by fatilly           #+#    #+#             */
/*   Updated: 2022/08/23 03:30:05 by fatilly          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

static bool		isUserExist(std::string name, std::vector<Socket> sockets)
{
	unsigned long int	i = 0;
	
	while (i < sockets.size())
	{
		if (name == sockets[i].getUser()->getNick())
		{
			if (sockets[i].getUser()->getRegistered() == true)
				return (true);
			else
				return (false);
		}
		i++;
	}
	return (false);
}

static User*	findAuthor(std::string name, std::vector<Socket> sockets)
{
	unsigned long int	i = 0;
	std::vector<Socket>::iterator it;
	
	it = sockets.begin();
	while ((name != sockets[i].getUser()->getNick()) && it != sockets.end())
	{
		it++;
		i++;
	}
	return (sockets[i].getUser());
}

static bool	ischannelExists(Message& msg, std::string chan)
{
	unsigned long int i = 0;
	
	while (i < msg.getServer()->getChannels().size())
	{
		if (!chan.compare(msg.getServer()->getChannels()[i]->getName()))
			return (true);
		i++;
	}
	return (false);
}

static unsigned long int	findChannel(Message& msg, std::string chan)
{
	unsigned long int i = 0;
	
	while (i < msg.getServer()->getChannels().size())
	{
		if (!chan.compare(msg.getServer()->getChannels()[i]->getName()))
			return (i);
		i++;
	}
	return (i);
}

void	invite_cmd(Message& msg)
{
	unsigned long int i;

	if (msg.getAuthor()->getRegistered() == false)
		sendErrorMessage(msg, NOAUTH, msg.getAuthor()->getNick(), "You are not registered");
	else if (msg.getArgs().size() != 3)
		sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
	else
	{
		if (ischannelExists(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1)) == true)
		{
			i = findChannel(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1));
			if (isUserExist(msg.getArgs()[2], msg.getServer()->getSockets()))
				msg.getServer()->getChannels()[i]->inviteUser(*msg.getAuthor(), *findAuthor(msg.getArgs()[2], msg.getServer()->getSockets()));
			else
				sendErrorMessage(msg, NONICK, msg.getAuthor()->getNick(), "No such nick");
		}
		else
				sendErrorMessage(msg, NOCHAN, msg.getArgs()[1], "No such channel");
	}
}
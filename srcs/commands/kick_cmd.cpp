/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpuccion <dpuccion@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 22:16:29 by fatilly           #+#    #+#             */
/*   Updated: 2022/08/23 05:25:27 by dpuccion         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

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

static 	User*	findUser(std::string nick, std::vector<Socket> sockets)
{
	unsigned long int	i = 0;

	while (i < sockets.size())
	{
		if(nick == sockets[i].getUser()->getNick())
			return (sockets[i].getUser());
		i++;
	}
	return (sockets[i].getUser());
}

static bool	UserExist(Message &msg, std::string target)
{
	unsigned long int i = 0;
	
	while (i < msg.getServer()->getSockets().size())
	{
		if (!target.compare(msg.getServer()->getSockets()[i].getUser()->getNick()))
			return (true);
		i++;
	}
	return (false);
}

void	kick_cmd(Message& msg)
{
	unsigned long int 	i;

	if (msg.getAuthor()->getRegistered() == false)
		sendErrorMessage(msg, NOAUTH, msg.getAuthor()->getNick(), "You are not registered");
	else if (msg.getArgs().size() != 4)
		sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
	else
	{
		if (ischannelExists(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1)))
		{
			i = findChannel(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1));
			if (UserExist(msg, msg.getArgs()[2]))
				msg.getServer()->getChannels()[i]->kickUser(*msg.getAuthor(), *findUser(msg.getArgs()[2], msg.getServer()->getSockets()), msg.getArgs()[3]);
			else
				sendErrorMessage(msg, NONICK, msg.getAuthor()->getNick(), "No suck nick");
		}
		else
			sendErrorMessage(msg, NOCHAN, msg.getArgs()[1], "No such channel");
	}
}
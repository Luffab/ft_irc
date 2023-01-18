/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatilly <fatilly@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 05:32:27 by fatilly           #+#    #+#             */
/*   Updated: 2022/08/23 03:30:34 by fatilly          ###   ########lyon.fr   */
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

void	part_cmd(Message& msg)
{
	unsigned long int i;

	if (msg.getAuthor()->getRegistered() == false)
		sendErrorMessage(msg, NOAUTH, msg.getAuthor()->getNick(), "You are not registered");
	else if (msg.getArgs().size() != 2)
		sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
	else
	{
		if (ischannelExists(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1)))
		{
			i = findChannel(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1));
			msg.getServer()->getChannels()[i]->leaveChannel(*msg.getAuthor());
		}
		else
			sendErrorMessage(msg, NOCHAN, msg.getArgs()[1], "No such channel");
	}
}
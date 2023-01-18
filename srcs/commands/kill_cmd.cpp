/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatilly <fatilly@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 05:06:04 by fatilly           #+#    #+#             */
/*   Updated: 2022/08/23 03:30:22 by fatilly          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

int	findUser(Message& msg, std::string target)
{
	unsigned long int i = 0;
	
	while (i < msg.getServer()->getSockets().size())
	{
		if (!target.compare(msg.getServer()->getSockets()[i].getUser()->getNick()))
			return (msg.getServer()->getSockets()[i].getID());
		i++;
	}
	return (msg.getServer()->getSockets()[i].getID());
}

bool	UserExist(Message &msg, std::string target)
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

bool	isModo(Message &msg, std::string target)
{
	unsigned long int i = 0;
	
	while (i < msg.getServer()->getOperatorsUsers().size())
	{
		if (!target.compare(msg.getServer()->getOperatorsUsers()[i]->getNick()))
			return (true);
		i++;
	}
	return (false);
}

void	kill_cmd(Message& msg)
{
	int	id;

	if (msg.getAuthor()->getRegistered() == false)
		sendErrorMessage(msg, NOAUTH, msg.getAuthor()->getNick(), "You are not registered");
	else if (msg.getArgs().size() != 2)
		sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
	else if (!msg.getAuthor()->getModo())
		sendErrorMessage(msg, NOACCESS, msg.getAuthor()->getNick(), "You have no rights");
	else if (msg.getAuthor()->getId() == findUser(msg, msg.getArgs()[1]))
		sendErrorMessage(msg, SUICIDE, msg.getAuthor()->getNick(), "You cannot kill yourself");
	else
	{
		if (UserExist(msg, msg.getArgs()[1]) == true)
		{
			if (!isModo(msg, msg.getArgs()[1]))
			{
				id = findUser(msg, msg.getArgs()[1]);
				msg.getServer()->deleteUserinVectors(id);
				msg.getServer()->deleteUser(id);
				close(id);
				FD_CLR(id, msg.getServer()->getMaster());
			}
			else
				sendErrorMessage(msg, NOACCESS, msg.getAuthor()->getNick(), "You cannot kill an operator");
		}
	}
}
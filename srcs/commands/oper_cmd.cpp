/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatilly <fatilly@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 05:47:28 by fatilly           #+#    #+#             */
/*   Updated: 2022/08/23 03:30:32 by fatilly          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

bool	isUserinModoChannel(Message& msg, std::vector<User*> vector)
{
	unsigned long int			i = 0;

	while (i < vector.size())
	{
		if (msg.getAuthor()->getId() == vector[i]->getId())
			return (true);
		i++;
	}
	return (false);
}

void	addModoinOperChan(Message& msg)
{
	unsigned long int			i = 0;

	while (i < msg.getServer()->getChannels().size())
	{
		if (!isUserinModoChannel(msg, msg.getServer()->getChannels()[i]->getOperatorsVector()))
			msg.getServer()->getChannels()[i]->pushBackModoVector(*msg.getAuthor());
		if (!isUserinModoChannel(msg, msg.getServer()->getChannels()[i]->getVoicedVector()))
			msg.getServer()->getChannels()[i]->pushBackVoicedVector(*msg.getAuthor());
		i++;
	}
}

void	oper_cmd(Message& msg)
{
	if (msg.getAuthor()->getRegistered() == false)
		sendErrorMessage(msg, NOAUTH, msg.getAuthor()->getNick(), "You are not registered");
	else if (msg.getArgs().size() != 2)
		sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
	else if (msg.getAuthor()->getModo() == true)
		sendErrorMessage(msg, ALREADYOP, msg.getAuthor()->getNick(), "You are already operator");
	else if (msg.getServer()->getAdminPass() == msg.getArgs()[1])
	{
		msg.getServer()->setOperatorUser(*msg.getAuthor());
		addModoinOperChan(msg);
		msg.getAuthor()->setModo(true);
		sendMessage(msg, msg.getArgs()[0], "You are know a server operator", msg.getAuthor()->getId());
	}
	else
		sendErrorMessage(msg, WRONGPASS, msg.getAuthor()->getNick(), "Wrong password");
}
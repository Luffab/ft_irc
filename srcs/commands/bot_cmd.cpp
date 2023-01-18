/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_cmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatilly <fatilly@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 04:35:04 by fatilly           #+#    #+#             */
/*   Updated: 2022/08/23 04:54:18 by fatilly          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

std::string	sentences [] =	{	"Bande de salopiauds",
								"Hey oh les deglingos",
								"Fermez vos mouilles",
								"Prenez des douches svp ca pue",
								"Ca va les puceaux ?",
								"Mozarellaaaaaa di Buffalaaaaa",
								"Il pue du cul votre channel",
								"Ratio + L + hulule + pleure + chiale",
								"Vous auriez pas vu un fdp par ici ?",
								"C'est un channel la ou un zoo ?"
							};

bool	ischannelExists(Message& msg, std::string chan)
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

static unsigned long int	findChannel2(Message& msg, std::string chan)
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

void	bot_cmd(Message& msg)
{
	srand((unsigned int)time(NULL));
	std::vector<int> targets;

	int nb = std::rand() % 10;
	if (msg.getAuthor()->getRegistered() == false)
		sendErrorMessage(msg, NOAUTH, msg.getAuthor()->getNick(), "You are not registered");
	else if (msg.getArgs().size() != 2)
		sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
	else
	{
		if (msg.getArgs()[1] == "help")
			sendMessage(msg, msg.getArgs()[0], "Usage: 'TRASH #channel'", msg.getAuthor()->getId());
		else if (msg.getArgs()[1][0] != '#')
			sendErrorMessage(msg, CHANWRITE, msg.getAuthor()->getNick(), "Write channel with char '#'");
		else if (ischannelExists(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1)) == true)
		{
			msg.setBotContent(sentences[nb]);
			unsigned long int j = 0;
			unsigned long int i = findChannel2(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1));
			while (j < msg.getServer()->getChannels()[i]->getUsersVector().size())
			{
				if (msg.getServer()->getChannels()[i]->getUsersVector()[j]->getId() != msg.getAuthor()->getId())
					targets.push_back(msg.getServer()->getChannels()[i]->getUsersVector()[j]->getId());
				j++;
			}
			msg.setTargets(targets);
			sendMessage(msg, "PRIVMSG", msg.getBotContent(), msg.getAuthor()->getId());
		}
	}
}
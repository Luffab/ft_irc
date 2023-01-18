#include "Message.hpp"

unsigned long int	findChannel4(Message& msg, std::string chan)
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

bool	channelExists4(Message& msg, std::string chan)
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

static	bool	userExist(std::string nick, std::vector<Socket> sockets)
{
	unsigned long int	i = 0;

	while (i < sockets.size())
	{
		if(nick == sockets[i].getUser()->getNick())
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

void	bMode(unsigned long int i, Message &msg)
{
	if (msg.getArgs().size() != 4)
		sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
	else if (msg.getArgs()[2][0] == '+')
	{
		if (userExist(msg.getArgs()[3], msg.getServer()->getSockets()) == true)
			msg.getServer()->getChannels()[i]->bannUser(*msg.getAuthor(), *findUser(msg.getArgs()[3], msg.getServer()->getSockets()));
	}
	else if (msg.getArgs()[2][0] == '-')
		msg.getServer()->getChannels()[i]->debannUser(*msg.getAuthor(), *findUser(msg.getArgs()[3], msg.getServer()->getSockets()));
}

void	iMode(unsigned long int i, Message &msg)
{
	if (msg.getArgs().size() != 3)
		sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
	else if (msg.getArgs()[2][0] == '+')
		msg.getServer()->getChannels()[i]->inviteOnly(*msg.getAuthor());
	else if (msg.getArgs()[2][0] == '-')
		msg.getServer()->getChannels()[i]->removeInviteOnly(*msg.getAuthor());
}

void	kMode(unsigned long int i, Message &msg)
{
	if (msg.getArgs()[2][0] == '+')
	{
		if (msg.getArgs().size() != 4)
			sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
		else
			msg.getServer()->getChannels()[i]->addKeyProtect(*msg.getAuthor(), msg.getArgs()[3]);
	}
	else if (msg.getArgs()[2][0] == '-')
	{
		if (msg.getArgs().size() != 3)
			sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
		else
			msg.getServer()->getChannels()[i]->removeKeyProtect(*msg.getAuthor());
	}
}

void	lMode(unsigned long int i, Message &msg)
{
	if (msg.getArgs()[2][0] == '+')
	{
		if (msg.getArgs().size() != 4)
			sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
		else
			msg.getServer()->getChannels()[i]->addLimitUsers(*msg.getAuthor(), (unsigned long int)atoi(msg.getArgs()[3].c_str()));
	}
	else if (msg.getArgs()[2][0] == '-')
	{
		if (msg.getArgs().size() != 3)
			sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
		else
			msg.getServer()->getChannels()[i]->removeLimitUsers(*msg.getAuthor());
	}
}

void	oMode(unsigned long int i, Message &msg)
{
	if (msg.getArgs().size() != 4)
		send(msg.getAuthor()->getId(), "Mauvais nombre d'arguments.\n", 28, 0);
	else
	{
		if (userExist(msg.getArgs()[3], msg.getServer()->getSockets()))
		{
			if (msg.getArgs()[2][0] == '+')
				msg.getServer()->getChannels()[i]->addOperator(*msg.getAuthor(), *findUser(msg.getArgs()[3], msg.getServer()->getSockets()));
			else if (msg.getArgs()[2][0] == '-')
				msg.getServer()->getChannels()[i]->removeOperator(*msg.getAuthor(), *findUser(msg.getArgs()[3], msg.getServer()->getSockets()));
		}
	}
}

void	tMode(unsigned long int i, Message &msg)
{
	if (msg.getArgs().size() != 3)
		sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
	else if (msg.getArgs()[2][0] == '+')
		msg.getServer()->getChannels()[i]->addTopicProtection(*msg.getAuthor());
	else if (msg.getArgs()[2][0] == '-')
		msg.getServer()->getChannels()[i]->removeTopicProtection(*msg.getAuthor());
}

void	nMode(unsigned long int i, Message &msg)
{
	if (msg.getArgs().size() != 3)
		sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
	else if (msg.getArgs()[2][0] == '+')
		msg.getServer()->getChannels()[i]->addNoExternalMessage(*msg.getAuthor());
	else if (msg.getArgs()[2][0] == '-')
		msg.getServer()->getChannels()[i]->removeNoExternalMessage(*msg.getAuthor());
}

void	mMode(unsigned long int i, Message &msg)
{
	if (msg.getArgs().size() != 3)
		sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
	else if (msg.getArgs()[2][0] == '+')
		msg.getServer()->getChannels()[i]->addModerated(*msg.getAuthor());
	else if (msg.getArgs()[2][0] == '-')
		msg.getServer()->getChannels()[i]->removeModerated(*msg.getAuthor());
}

void	vMode(unsigned long int i, Message &msg)
{
	if (msg.getArgs().size() != 4)
		send(msg.getAuthor()->getId(), "Mauvais nombre d'arguments.\n", 28, 0);
	else
	{
		if (userExist(msg.getArgs()[3], msg.getServer()->getSockets()))
		{
			if (msg.getArgs()[2][0] == '+')
				msg.getServer()->getChannels()[i]->addVoicedUser(*msg.getAuthor(), *findUser(msg.getArgs()[3], msg.getServer()->getSockets()));
			else if (msg.getArgs()[2][0] == '-')
				msg.getServer()->getChannels()[i]->removeVoicedUser(*msg.getAuthor(), *findUser(msg.getArgs()[3], msg.getServer()->getSockets()));
		}
	}
}

void	modeSyntax(Message &msg)
{
	if (msg.getArgs()[1][0] == '#')
	{
		if (channelExists4(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1)) == true)
		{
			unsigned long int i = findChannel4(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1));
			if (msg.getArgs()[2][0] != '+' && msg.getArgs()[2][0] != '-')
				sendMessage(msg, msg.getArgs()[0], "Please specify + or -", msg.getAuthor()->getId());
			else if ( msg.getArgs()[2][2])
				sendMessage(msg, msg.getArgs()[0], "Please specify flag b, i, k, l, m, n, o, t or v", msg.getAuthor()->getId());
			else if (msg.getArgs()[2][1] == 'b')
				bMode(i, msg);
			else if (msg.getArgs()[2][1] == 'i')
				iMode(i, msg);
			else if (msg.getArgs()[2][1] == 'k')
				kMode(i, msg);
			else if (msg.getArgs()[2][1] == 'l')
				lMode(i, msg);
			else if (msg.getArgs()[2][1] == 'm')
				mMode(i, msg);
			else if (msg.getArgs()[2][1] == 'n')
				nMode(i, msg);
			else if (msg.getArgs()[2][1] == 'o')
				oMode(i, msg);
			else if (msg.getArgs()[2][1] == 't')
				tMode(i, msg);
			else if (msg.getArgs()[2][1] == 'v')
				vMode(i, msg);
			else
				sendMessage(msg, msg.getArgs()[0], "Please specify flag b, i, k, l, m, n, o, t or v", msg.getAuthor()->getId());
		}
		else
			sendErrorMessage(msg, NOCHAN, msg.getArgs()[1], "No such channel");
	}
}

void	mode_cmd(Message &msg)
{
	if (msg.getAuthor()->getRegistered() == false)
		sendErrorMessage(msg, NOAUTH, msg.getAuthor()->getNick(), "You are not registered");
	else
		modeSyntax(msg);
}
#include "Message.hpp"

unsigned long int	findTarget(Message& msg, std::string target)
{
	unsigned long int i = 0;
	
	while (i < msg.getServer()->getSockets().size())
	{
		if (!target.compare(msg.getServer()->getSockets()[i].getUser()->getNick()))
			return (i);
		i++;
	}
	return (i);
}

bool	targetExist(Message &msg, std::string target)
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

unsigned long int	findChannel2(Message& msg, std::string chan)
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

bool	channelExists2(Message& msg, std::string chan)
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

static bool	isInBannChannels(std::vector<Channel> &bannChan, std::string chan)
{
	unsigned long int i = 0;
	while (i < bannChan.size())
	{
		if (chan == bannChan[i].getName())
			return (true);
		i++;
	}
	return (false);
}

static bool	isInVoicedVector(std::vector<User *> &voicedUsers, std::string user)
{
	unsigned long int i = 0;
	while (i < voicedUsers.size())
	{
		if (user == voicedUsers[i]->getNick())
			return (true);
		i++;
	}
	return (false);
}

static bool	isNotInChannel(Message &msg, unsigned long int i)
{
	unsigned long int j = 0;

	while (j < msg.getServer()->getChannels()[i]->getUsersVector().size())
	{
		if (msg.getAuthor()->getNick() ==  msg.getServer()->getChannels()[i]->getUsersVector()[j]->getNick())
			return (false);
		j++;
	}
	return (true);
}

void	privmsgSyntax(Message &msg)
{
	std::vector<int> targets;
	if (msg.getArgs()[1][0] == '#')
	{
		if (channelExists2(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1)) == true)
		{
			unsigned long int i = findChannel2(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1));
			unsigned long int j = 0;
			if (isInBannChannels(msg.getAuthor()->getBanChannels(),  msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1)) == true)
				sendErrorMessage(msg, ALREADYBAN, msg.getAuthor()->getNick(), "You are ban of this channel");
			else if (isNotInChannel(msg, i) == true && msg.getServer()->getChannels()[i]->getHasNoExtMsg() == true)
				sendErrorMessage(msg, NOEXTMSG, msg.getAuthor()->getNick(), "This channel is in no external message mode");
			else if ((isInVoicedVector(msg.getServer()->getChannels()[i]->getVoicedVector(),  msg.getAuthor()->getNick()) == false) && msg.getServer()->getChannels()[i]->getIsModerated() == true)
				sendErrorMessage(msg, NOVOICE, msg.getAuthor()->getNick(), "You don't have the voice in this channel");
			else
			{
				while (j < msg.getServer()->getChannels()[i]->getUsersVector().size())
				{
					if (msg.getServer()->getChannels()[i]->getUsersVector()[j]->getId() != msg.getAuthor()->getId())
						targets.push_back(msg.getServer()->getChannels()[i]->getUsersVector()[j]->getId());
					j++;
				}
				msg.setTargets(targets);
			}
		}
		else
			sendErrorMessage(msg, NOCHAN, msg.getArgs()[1], "No such channel");
	}
	else
	{
		if (targetExist(msg, msg.getArgs()[1]) == true)
		{
			targets.push_back(msg.getServer()->getSockets()[findTarget(msg, msg.getArgs()[1])].getID());
			msg.setTargets(targets);
		}
		else
			sendErrorMessage(msg, NONICK, msg.getAuthor()->getNick(), "No such nick");
	}
}

void	privmsg_cmd(Message &msg)
{
	if (msg.getAuthor()->getRegistered() == false)
		sendErrorMessage(msg, NOAUTH, msg.getAuthor()->getNick(), "You are not registered");
	else
	{
		if (msg.getArgs().size() != 3)
    		sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
		else
			privmsgSyntax(msg);
	}
}
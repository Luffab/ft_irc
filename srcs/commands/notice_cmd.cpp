#include "Message.hpp"

unsigned long int	findTarget2(Message& msg, std::string target)
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

bool	targetExist2(Message &msg, std::string target)
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

unsigned long int	findChannel3(Message& msg, std::string chan)
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

bool	channelExists3(Message& msg, std::string chan)
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

void	noticeSyntax(Message &msg)
{
	std::vector<int> targets;
	if (msg.getArgs()[1][0] == '#')
	{
		if (channelExists3(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1)) == true)
		{
			unsigned long int i = findChannel3(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1));
			unsigned long int j = 0;
			if (isInBannChannels(msg.getAuthor()->getBanChannels(),  msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1)) == true)
				return ;
			else if (isNotInChannel(msg, i) == true && msg.getServer()->getChannels()[i]->getHasNoExtMsg() == true)
				return ;
			else if ((isInVoicedVector(msg.getServer()->getChannels()[i]->getVoicedVector(),  msg.getAuthor()->getNick()) == false) && msg.getServer()->getChannels()[i]->getIsModerated() == true)
				return ;
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
	}
	else
	{
		if (targetExist2(msg, msg.getArgs()[1]) == true)
		{
			targets.push_back(msg.getServer()->getSockets()[findTarget2(msg, msg.getArgs()[1])].getID());
			msg.setTargets(targets);
		}
	}
}

void	notice_cmd(Message &msg)
{
	if (msg.getAuthor()->getRegistered() == false)
		sendErrorMessage(msg, NOAUTH, msg.getAuthor()->getNick(), "You are not registered");
	else
	{
		if (msg.getArgs().size() != 3)
    		return ;
		else
			noticeSyntax(msg);
	}
}
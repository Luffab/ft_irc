#include "Message.hpp"

unsigned long int	findChannel(Message& msg, std::string chan)
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

bool	channelExists(Message& msg, std::string chan)
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

void	join_cmd(Message &msg)
{
	if (msg.getAuthor()->getRegistered() == false)
		sendErrorMessage(msg, NOAUTH, msg.getAuthor()->getNick(), "You are not registered");
	else if (msg.getArgs().size() != 2 && msg.getArgs().size() != 3)
		sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
	else
	{
		if (msg.getArgs()[1][0] != '#')
			sendErrorMessage(msg, CHANWRITE, msg.getAuthor()->getNick(), "Write channel with char '#'");
		else if (channelExists(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1)) == false)
		{
			if (msg.getArgs().size() != 2)
				sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
			else
			{
				msg.getServer()->addChannel(msg);
				sendMessage(msg, msg.getArgs()[0], "Channel created", msg.getAuthor()->getId());
			}
		}
		else
		{
			unsigned long int indice = findChannel(msg, msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1));
			if (msg.getServer()->getChannels()[indice]->getHasPassword() == false && msg.getArgs().size() != 2)
			{
				sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
				return ;
			}
			if (msg.getServer()->getChannels()[indice]->getHasPassword() == true && msg.getArgs().size() != 3)
			{
				sendErrorMessage(msg, WRONGPASS, msg.getAuthor()->getNick(), "Password required");
				return ;
			}
			if (msg.getServer()->getChannels()[indice]->getIsInviteOnly() == true)
				sendErrorMessage(msg, NOACCESS, msg.getAuthor()->getNick(), "This channel is in invite only");
			else if (isInBannChannels(msg.getAuthor()->getBanChannels(),  msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1)) == true)
				sendErrorMessage(msg, ALREADYBAN, msg.getAuthor()->getNick(), "You are banned from this channel");
			else
			{
				if (msg.getServer()->getChannels()[indice]->getHasPassword() == false)
					msg.getServer()->getChannels()[indice]->joinChannel(*msg.getAuthor());
				else
				{
					if (msg.getArgs()[2] == msg.getServer()->getChannels()[indice]->getPassword())
						msg.getServer()->getChannels()[indice]->joinChannel(*msg.getAuthor());
					else
						sendErrorMessage(msg, WRONGPASS, msg.getAuthor()->getNick(), "Wrong password");
				}
			}
		}
	}
}
#include "Message.hpp"

bool	nickExists(Message& msg, std::string nick)
{
	unsigned long int i = 0;
	
	while (i < msg.getServer()->getSockets().size())
	{
		if (!nick.compare(msg.getServer()->getSockets()[i].getUser()->getNick()))
			return (true);
		i++;
	}
	return (false);
}

bool	nickIsValid(Message& msg, std::string nick)
{
	if (nick.size() > 9)
	{
		sendErrorMessage(msg, BADNICK, "USER", "Nickname too long (9 char max)");
		return (false);
	}
	else if (nickExists(msg, nick) == true)
	{
		sendErrorMessage(msg, NICKEXIST, "USER", "Nickname already used");
		return (false);
	}
	else
		return (true);
}

void	nick_cmd(Message& msg)
{
	if (msg.getArgs().size() != 2)
            sendErrorMessage(msg, NBARGS, "USER", "Bad arguments number");
    else
    {
		if (msg.getAuthor()->getAuth() == false)
			 sendErrorMessage(msg, NOPASS, "USER", "Type first the password with PASS command");
		else
		{
			if (nickIsValid(msg, msg.getArgs()[1]) == true)
			{
				msg.getAuthor()->setNick(msg.getArgs()[1]);
				sendMessage(msg, msg.getArgs()[0], "Nickname saved", msg.getAuthor()->getId());
			}
		}
    }
}
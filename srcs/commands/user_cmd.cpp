#include "Message.hpp"

void	user_cmd(Message &msg)
{
	if (msg.getArgs().size() != 5)
        sendErrorMessage(msg, NBARGS, msg.getAuthor()->getNick(), "Bad arguments number");
	else
	{
		if (msg.getAuthor()->getAuth() == false)
			sendErrorMessage(msg, NOPASS, msg.getAuthor()->getNick(), "Type first the password with PASS command");
		else if (msg.getAuthor()->getRegistered() == true)
			sendErrorMessage(msg, ALREADYID, msg.getAuthor()->getNick(), "You are already registered");
		else
		{
			if (msg.getAuthor()->getNick().size() < 1)
				sendErrorMessage(msg, FIRSTNICK, "USER", "First choose a nickname with NICK command");
			else
			{
				msg.getAuthor()->setUsername(msg.getArgs()[1]);
				msg.getAuthor()->setHostname(msg.getArgs()[2]);
				msg.getAuthor()->setServname(msg.getArgs()[3]);
				msg.getAuthor()->setRealname(msg.getArgs()[4]);
				msg.getAuthor()->setRegistered(true);
				sendMessage(msg, msg.getArgs()[0], "You are registered", msg.getAuthor()->getId());
			}
		}
	}
}
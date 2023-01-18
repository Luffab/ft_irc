#include "Message.hpp"

void	pass_cmd(Message& msg)
{
	if (msg.getArgs().size() != 2)
            sendErrorMessage(msg, NBARGS, "USER", "Bad arguments number");
    else
    {
		if (msg.getAuthor()->getAuth() == true)
			sendErrorMessage(msg, ALREADYID, "USER", "You are already authentified");
        else if (msg.getArgs()[1].compare(msg.getServer()->getPassword()))
			sendErrorMessage(msg, WRONGPASS, "USER", "Wrong password");
		else
			msg.getAuthor()->setAuth(true);
    }
}

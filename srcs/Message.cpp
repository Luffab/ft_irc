#include "Message.hpp"

Message::Message(std::string buf, Server *server, User *author): content(buf), server(server), author(author) {}

Message::~Message() {}

Server*	Message::getServer(void)
{
	return (this->server);
}

User*	Message::getAuthor(void)
{
	return (this->author);
}

std::vector<std::string>	Message::getArgs(void)
{
	return (this->cmds_args);
}

std::string	Message::getBotContent()
{
	return (bot_content);
}

std::vector<int>&	Message::getTargets(void)
{
	return (this->_targets);
}

void	Message::setAuthor(User* ref)
{
	this->author = ref;
}

void	Message::setBotContent(std::string str)
{
	bot_content = str;
}

void	Message::setTargets(std::vector<int> &targets)
{
	this->_targets = targets;
}

bool    Message::parsing()
{
	if (get_cmds_args() == false || this->cmds_args.size() == 0)
		return (false);
    what_cmd();
	std::cout << std::endl;
	std::cout << "[" << this->content << "Message sent by user identified by nick '" << this->author->getNick() << "' and ID '" << this->author->getId() << "']" << std::endl << std::endl;
	return (true);
}

bool    Message::get_cmds_args()
{
	long unsigned int pos = 0;
	long unsigned int start = 0;

	while(content[pos])
	{
		if (content[pos] == ' ' || content[pos] == '\r' || content[pos] == '\n')
			pos++;
		else if (content[pos] == ':')
		{
			pos++;
			start = pos;
			while (content[pos] != '\r' && content[pos] != '\n' && content[pos])
				pos++;
			this->cmds_args.push_back(this->content.substr(start, pos - start));
			break ;
		}
		else
		{
			start = pos;
			while (content[pos] != ' ' && content[pos] != '\r' && content[pos] != '\n' && content[pos])
				pos++;
			this->cmds_args.push_back(this->content.substr(start, pos - start));
			pos++;
		}
	}
	return (true);
}

void    Message::what_cmd()
{
    if (!this->cmds_args[0].compare("PASS"))
		pass_cmd(*this);
	else if (!this->cmds_args[0].compare("NICK"))
		nick_cmd(*this);
	else if (!this->cmds_args[0].compare("USER"))
		user_cmd(*this);
	else if (!this->cmds_args[0].compare("JOIN"))
		join_cmd(*this);
	else if (!this->cmds_args[0].compare("PRIVMSG"))
		privmsg_cmd(*this);
	else if (!this->cmds_args[0].compare("KILL"))
		kill_cmd(*this);
	else if (!this->cmds_args[0].compare("OPER"))
		oper_cmd(*this);
	else if (!this->cmds_args[0].compare("MODE"))
		mode_cmd(*this);
	else if (!this->cmds_args[0].compare("TRASH"))
		bot_cmd(*this);
	else if (!this->cmds_args[0].compare("TOPIC"))
		topic_cmd(*this);
	else if (!this->cmds_args[0].compare("INVITE"))
		invite_cmd(*this);
	else if (!this->cmds_args[0].compare("PART"))
		part_cmd(*this);
	else if (!this->cmds_args[0].compare("NOTICE"))
		notice_cmd(*this);
	else if (!this->cmds_args[0].compare("KICK"))
		kick_cmd(*this);
	else
		sendErrorMessage(*this, WRONGCMD, author->getNick(), "Wrong command");
}

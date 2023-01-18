#pragma once

#include "Server.hpp"
#include "User.hpp"
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <time.h>

class 	Server;
class	User;

class Message
{
	private:
        std::string content;
		std::string	bot_content;
		std::vector<std::string> cmds_args;
		std::vector<int>		_targets;
		Server	*server;
		User	*author;

        bool   	get_cmds_args();
		void	what_cmd();
		bool	nickIsValid(std::string nick);
		bool	nickExists(std::string nick);

	public:
		Message(std::string, Server *server, User *author);
		~Message();

		Server*						getServer(void);
		User*						getAuthor(void);
		std::vector<std::string>	getArgs(void);
		std::string					getBotContent();
		void						setAuthor(User* ref);
		void						setBotContent(std::string str);
		std::vector<int>&			getTargets(void);
		void						setTargets(std::vector<int> &targets);
        bool    					parsing();
};

void	pass_cmd(Message &msg);
void	nick_cmd(Message &msg);
void	user_cmd(Message &msg);
void	join_cmd(Message &msg);
void	bot_cmd(Message &msg);
void	privmsg_cmd(Message &msg);
void	notice_cmd(Message& msg);
void	kill_cmd(Message &msg);
void	oper_cmd(Message &msg);
void	mode_cmd(Message &msg);
void	topic_cmd(Message& cmd);
void	invite_cmd(Message& msg);
void	part_cmd(Message& msg);
void	kick_cmd(Message& msg);

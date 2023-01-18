#pragma once

#include <iostream>
#include <vector>
#include "User.hpp"
#include "Server.hpp"

class 	User;
class	Server;

class Channel
{
	private:
		std::vector<User*>	_users;
		std::vector<User*>	_banned_users;
		std::vector<User*>	_operators;
		std::vector<User*>	_voiced;
		std::string			_name;
		std::string			_topic;
		std::string			_password;
		unsigned long int	_max_users;
		bool				_is_moderated;
		bool				_is_invite_only;
		bool				_has_password;
		bool				_is_max_users;
		bool				_has_no_ext_msg;
		bool				_has_topic_protection;
		Server*				_server;

		std::vector<User*>::iterator	searchUser(User& user, std::vector<User*> &vector);
		int							isUserExist(User& user, std::vector<User*> vector);
		void						addModoInOperator(void);
		void						addModoInVoiced(void);

	public:
		Channel(std::string name, Server* server, User& user);
		~Channel();

		void	joinChannel(User& user);
		void	leaveChannel(User& user);
		void	kickUser(User& op_user, User& bann_user, std::string explaination);
		void	addTopic(std::string topic, User& user);
		void	removeTopic(User& user);
		void	addTopicProtection(User& op_user);
		void	removeTopicProtection(User& op_user);
		void	bannUser(User& op_user, User& bann_user);
		void	debannUser(User& op_user, User& debann_user);
		void	inviteOnly(User& user);
		void	removeInviteOnly(User& user);
		void	addKeyProtect(User& user, std::string password);
		void	removeKeyProtect(User& user);
		void	addLimitUsers(User& user, unsigned long int nb);
		void	removeLimitUsers(User& user);
		void	addModerated(User& user);
		void	removeModerated(User& user);
		void	addNoExternalMessage(User& user);
		void	removeNoExternalMessage(User& user);
		void	addOperator(User& op_user, User& user);
		void	removeOperator(User& op_user, User& user);
		void	addVoicedUser(User& op_user, User& user);
		void	removeVoicedUser(User& op_user, User& user);

		void	inviteUser(User& op_user, User& user);


		int								getMaxUsers();
		bool							getIsInviteOnly();
		bool							getHasPassword();
		bool							getIsMaxUsers();
		bool							getHasNoExtMsg();
		bool							getIsModerated();
		std::string						getName();
		std::string						getTopic();
		std::string						getPassword();
		int								getIfUserExist(User& user, std::vector<User*> vector);
		std::vector<User*>::iterator&	getUser();
		std::vector<User*>&				getOperatorsVector();
		std::vector<User*>&				getUsersVector();
		std::vector<User*>&				getBannUsersVector();
		std::vector<User*>&				getVoicedVector();
		void							pushBackModoVector(User& user);
		void							pushBackVoicedVector(User& user);

		void						deleteUserById(int id, std::vector<User*>& vector);
};

void	sendChannelErrorMessage(User& user, std::string code, std::string dest, std::string message);
void	sendChannelMessage(User& user, std::string message, std::string arg, int j);
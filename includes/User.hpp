#pragma once

#include <string>
#include "Channel.hpp"

class	Channel;

class User
{
	private:
		bool 					_is_authentified;
		bool					_is_modo;
		bool					_is_registered;
		std::string				_username;
		std::string				_hostname;
		std::string				_servname;
		std::string				_realname;
		std::vector<Channel>	_bann_channels;
		std::string				_nick;
		int						_id;

	public:
		User(int id);
		~User();

		bool 		getAuth();
		std::string	getNick();
		std::string	getUsername();
		std::string	getHostname();
		int			getId();
		int			getModo();
		bool		getRegistered();
		std::vector<Channel>	&getBanChannels();
		void		setRegistered(bool registered);
		void		setAuth(bool auth);
		void		setNick(std::string);
		void		setRegister(bool registered);
		void		setUsername(std::string);
		void		setHostname(std::string);
		void		setServname(std::string);
		void		setRealname(std::string);
		void		setModo(bool is_modo);

		void							pushBackBannChannels(Channel &channel);
		std::vector<Channel>::iterator	searchChannel(Channel& channel, User& user);
		void							removeBannChannel(Channel& channel);
};
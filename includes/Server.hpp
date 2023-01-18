#pragma once

#include "Message.hpp"
#include "User.hpp"
#include "Socket.hpp"
#include "Channel.hpp"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <fstream>

#define BUFSIZE	1024

#define NONICK			"401"
#define	NOCHAN			"403"
#define NOAUTH			"404"
#define NBARGS			"405"
#define CHANWRITE		"406"
#define	NOACCESS		"407"
#define	SUICIDE			"409"
#define	NOPASS			"411"
#define	ALREADYOP		"412"
#define	WRONGPASS		"413"
#define	ALREADYID		"414"
#define	FIRSTNICK		"421"
#define	BADNICK			"422"
#define	NICKEXIST		"423"
#define ALREADYINCHAN	"424"
#define HAVEVOICE		"431"
#define	ISCHANFULL		"432"
#define	ALREADYBAN		"433"
#define	NOEXTMSG		"434"
#define	NOVOICE			"435"
#define	WRONGCMD		"436"

class	Message;
class	Socket;
class	User;
class	Channel;

class Server
{
	private:
		fd_set					_master;
		fd_set					_read_fds;
		int 					_fdmax;
		int						_i;
		int 					_sockfd;
		uint16_t				_port;
		std::string				_password;
		std::string				_admin_pass;	
		struct sockaddr_in		_my_addr;
		struct sockaddr_in		_client_addr;
		std::vector<Socket>		_sockets;
		std::vector<User*>		_operators_users;
		std::vector<Channel*>	_channels;
		int						_is_entier;
		ssize_t					_buf_size;

		void	SendToAll(int j, int i, int sockfd, fd_set *master, Message &msg);
		void 	SendRecv(int i, fd_set *master, int sockfd, int fdmax, char *final_buf);
		int		ConnectionAccept(fd_set *master, int *fdmax, int sockfd, struct sockaddr_in *client_addr);
		int		ConnectRequest(int *sockfd, struct sockaddr_in *my_addr);

	public:
		Server(uint16_t port, std::string password);
		~Server();

		void					setPassword(std::string str);
		void					setOperatorUser(User& user);
		std::string				getPassword();
		std::string				getAdminPass();
		std::vector<Socket> 	getSockets();
		std::vector<User*>&		getOperatorsUsers();
		std::vector<Channel*>&	getChannels();
		fd_set*					getMaster();
		int						OpenServer(void);
		void					addAdmin(User& user, std::string admin_pass);
		void					addChannel(Message& msg);
		void					deleteUser(int id);
		void					deleteUserinVectors(int id);
};

User*		findAuthor(int id, std::vector<Socket> sockets);
Channel*	findChannel(std::string name, std::vector<Channel> channel);
void		sendMessage(Message& msg, std::string message, std::string arg, int j);
void		sendErrorMessage(Message &msg, std::string code, std::string dest, std::string message);
int			isChannelExist(std::string name, std::vector<Channel> channel);
char		*ft_strjoin(char *s1, char *s2);
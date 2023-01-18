#include "Server.hpp"

Server::Server(uint16_t port, std::string password): _port(port), _password(password), _admin_pass("root") {}

Server::~Server()
{
	for (unsigned int i = 0; i < _channels.size(); i++)
		delete (&_channels[i]);
}

void	Server::setPassword(std::string str)
{
	this->_password = str;
}

void	Server::setOperatorUser(User& user)
{
	_operators_users.push_back(&user);
}

std::string	Server::getPassword(void)
{
	return (this->_password);
}

std::vector<Socket> Server::getSockets()
{
	return (this->_sockets);
}

std::vector<User*>&	Server::getOperatorsUsers()
{
	return (_operators_users);
}

std::vector<Channel*>& Server::getChannels()
{
	return (this->_channels);
}

fd_set*	Server::getMaster()
{
	return (&_master);
}

std::string	Server::getAdminPass()
{
	return (_admin_pass);
}

void	Server::addChannel(Message& msg)
{
	Channel* tmp = new Channel(msg.getArgs()[1].substr(1, msg.getArgs()[1].size() - 1), msg.getServer(), *msg.getAuthor());
	this->_channels.push_back(tmp);
}

void	Server::deleteUser(int id)
{
	unsigned long int				j = 0;
	std::vector<Socket>::iterator	it;
	
	it = _sockets.begin();
	while ((id != _sockets[j].getID()) && it != _sockets.end())
	{
		it++;
		j++;
	}
	delete(_sockets[j].getUser());
	j = 0;
	it = _sockets.begin();
	while ((id != _sockets[j].getID()) && it != _sockets.end())
	{
		it++;
		j++;
	}
	_sockets.erase(it);

}

void	Server::deleteUserinVectors(int id)
{
	unsigned long int				j = 0;
	std::vector<Channel*>::iterator	it;
	std::vector<User*>::iterator		usr_it;

	it = _channels.begin();
	while (j < _channels.size())
	{
		_channels[j]->deleteUserById(id, *&_channels[j]->getOperatorsVector());
		_channels[j]->deleteUserById(id, *&_channels[j]->getUsersVector());
		_channels[j]->deleteUserById(id, *&_channels[j]->getBannUsersVector());
		_channels[j]->deleteUserById(id, *&_channels[j]->getVoicedVector());
		j++;
		it++;
	}
	usr_it = _operators_users.begin();
	j = 0;
	while (j < _operators_users.size())
	{
		if (id == _operators_users[j]->getId())
			_operators_users.erase(usr_it);
		usr_it++;
		j++;
	}
}

bool	isInTargets(int j, std::vector<int> &targets, std::vector<Socket> &sockets)
{
	unsigned long int	i = 0;

	while (i < sockets.size())
	{
		if (j == sockets[i].getID())
		{
			if (sockets[i].getUser()->getRegistered() == false)
				return (false);
		}
		i++;
	}
	i = 0;
	while (i < targets.size())
	{
		if (j == targets[i])
			return (true);
		i++;
	}
	return (false);
}

void	Server::SendToAll(int j, int i, int sockfd, fd_set *master, Message &msg)
{

	if (FD_ISSET(j, master))
	{
		if (j != sockfd && j != i)
		{
			if (msg.getArgs()[0] == "PRIVMSG" || msg.getArgs()[0] == "NOTICE")
			{
				if (isInTargets(j, msg.getTargets(), _sockets))
					sendMessage(msg, msg.getArgs()[0], msg.getArgs()[2], j);
			}
			else if (msg.getArgs()[0] == "TRASH")
			{
				if (isInTargets(j, msg.getTargets(), _sockets))
					sendMessage(msg, "PRIVMSG", msg.getBotContent(), j);
			}
		}
	}
}

void	Server::SendRecv(int i, fd_set *master, int sockfd, int fdmax, char *final_buf)
{
	ssize_t	nbytes_recvd;
	int		j;
	char	recv_buf[BUFSIZE];
	char	*temp_buf;

	memset(recv_buf, 0, 1024);
	if (_is_entier == 0)
	{
		temp_buf = (char *)malloc(sizeof(char) * BUFSIZE);
		memset(temp_buf, 0, 1024);
	}
	if ((nbytes_recvd = recv(i, recv_buf, BUFSIZE, 0)) <= 0)
	{
		if (nbytes_recvd == 0)
		{
			std::cout << "Le socket " << i << " est suspendu" << std::endl;
			if (temp_buf)
				free(temp_buf);
			_is_entier = 0;
			deleteUserinVectors(i);
			deleteUser(i);
		}
		else
			std::cerr << "Recv error" << std::endl;
		close(i);
		FD_CLR(i, master);
	}
	else
	{	
		if (nbytes_recvd > 512)
		{
			send(i, ":IRC 432 :The message must not overload 512 chars\n", 50, 0);
			return ;
		}
		if (recv_buf[nbytes_recvd - 1] != '\n')
		{
			_is_entier = 1;
			_buf_size = _buf_size + nbytes_recvd;
			temp_buf = ft_strjoin(temp_buf, recv_buf);
		}
		else
		{
			memset(final_buf, 0, 1024);
			if (_is_entier == 0)
			{
				_buf_size = nbytes_recvd;
				final_buf = strdup(recv_buf);
			}
			else if (_is_entier == 1)
			{
				final_buf = strdup(temp_buf);
				_buf_size += nbytes_recvd;
				final_buf = ft_strjoin(final_buf, recv_buf);
			}
			_is_entier = 0;
			memset(recv_buf, 0, 1024);
			Message tmp(final_buf, this, findAuthor(i, _sockets));
			if (tmp.parsing() == true)
			{
				for(j = 0; j <= fdmax; j++)
					SendToAll(j, i, sockfd, master, tmp);
			}
			_buf_size = 0;
			if (final_buf)
				free(final_buf);
			if (temp_buf)
				free(temp_buf);
		}
	}	
}

int	Server::ConnectionAccept(fd_set *master, int *fdmax, int sockfd, struct sockaddr_in *client_addr)
{
	socklen_t addrlen;
	int newsockfd;
	
	addrlen = sizeof(struct sockaddr_in);
	if((newsockfd = accept(sockfd, (struct sockaddr *)client_addr, &addrlen)) == -1)
	{
		std::cerr << "Accept error" << std::endl;
		return(0);
	}
	else
	{
		FD_SET(newsockfd, master);
		if(newsockfd > *fdmax)
			*fdmax = newsockfd;
		User* new_client = new User(newsockfd);
		Socket tmp(new_client, newsockfd);
		_sockets.push_back(tmp);
		std::cout << "Nouvelle connection de " << inet_ntoa(client_addr->sin_addr)
					<< " sur le port " << ntohs(client_addr->sin_port) << " avec l'ID " << newsockfd << std::endl;
	}
	return (1);
}
	
int	Server::ConnectRequest(int *sockfd, struct sockaddr_in *my_addr)
{
	int yes = 1;

	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Socket error" << std::endl;
		return (0);
	}
	my_addr->sin_family = AF_INET;
	my_addr->sin_port = htons(_port);
	my_addr->sin_addr.s_addr = INADDR_ANY;
	memset(my_addr->sin_zero, 0, sizeof(my_addr->sin_zero));
	if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		std::cerr << "Setsockopt error" << std::endl;
		return (0);
	}
	if (bind(*sockfd, (struct sockaddr *)my_addr, sizeof(struct sockaddr)) == -1)
	{
		std::cerr << "Bind error" << std::endl;
		return (0);
	}
	if (listen(*sockfd, 10) == -1)
	{
		std::cerr << "Listen error" << std::endl;
		return (0);
	}
	std::cout << "Le server attend un client sur le port " << _port <<std::endl;
	fflush(stdout);
	return (1);
}

int Server::OpenServer()
{
	char	*final_buf;

	FD_ZERO(&_master);
	FD_ZERO(&_read_fds);
	ConnectRequest(&_sockfd, &_my_addr);
	FD_SET(_sockfd, &_master);

	_fdmax = _sockfd;
	final_buf = (char *)malloc(sizeof(char) * BUFSIZE);
	_is_entier = 0;
	_buf_size = 0;
	while(1)
	{
		_read_fds = _master;
		if(select(_fdmax + 1, &_read_fds, NULL, NULL, NULL) == -1)
		{
			std::cerr << "Select error" << std::endl;
			return (0);
		}
		for (_i = 0; _i <= _fdmax; _i++)
		{
			if (FD_ISSET(_i, &_read_fds))
			{
				if (_i == _sockfd)
					ConnectionAccept(&_master, &_fdmax, _sockfd, &_client_addr);
				else
					SendRecv(_i, &_master, _sockfd, _fdmax, final_buf);
			}
		}
	}
	return (1);
}

void	Server::addAdmin(User& user, std::string admin_pass)
{
	if (admin_pass == _admin_pass)
	{
		_operators_users.push_back(&user);
		user.setModo(true);
	}
	else
		sendChannelErrorMessage(user, WRONGPASS, user.getNick(), "Wrong password");
}


User*	findAuthor(int id, std::vector<Socket> sockets)
{
	unsigned long int	i = 0;
	std::vector<Socket>::iterator it;
	
	it = sockets.begin();
	while ((id != sockets[i].getID()) && it != sockets.end())
	{
		it++;
		i++;
	}
	return (sockets[i].getUser());
}

Channel*	findChannel(std::string name, std::vector<Channel> channel)
{
	unsigned long int	i = 0;
	std::vector<Channel>::iterator it;

	it = channel.begin();
	while (i < channel.size())
	{
		if (channel[i].getName() == name)
			return (&channel[i]);
		it++;
		i++;
	}
	return (&channel[i]);
}

int	isChannelExist(std::string name, std::vector<Channel> channel)
{
	unsigned long int	i = 0;
	std::vector<Channel>::iterator it;

	it = channel.begin();
	while (i < channel.size())
	{
		if (channel[i].getName() == name)
			return (1);
		it++;
		i++;
	}
	return (0);
}

char*	ft_strjoin(char *s1, char *s2)
{
	int		i = 0;
	int		j = 0;
	char	*str;

	str = (char *)malloc(sizeof(char) * BUFSIZE);
	if (s1)
	{
		while (s1[i])
		{
			str[j] = s1[i];
			i++;
			j++;
		}
	}
	i = 0;
	while (s2[i])
	{
		str[j] = s2[i];
		i++;
		j++;
	}
	str[j] = 0;
	free(s1);
	return (str);
}

void	sendMessage(Message& msg, std::string message, std::string arg, int j)
{
	std::string	temp = message + " " + msg.getArgs()[1] + " :" + arg + "\n";
	std::string temp2 = ":" + msg.getAuthor()->getNick() + "!" + msg.getAuthor()->getUsername() + "@" + msg.getAuthor()->getHostname() + " " + temp;
	if (send(j, temp2.c_str(), temp2.size(), 0) == -1)
		std::cerr << "Send error" << std::endl;
}

void	sendErrorMessage(Message& msg, std::string code, std::string dest, std::string message)
{
	std::string temp = ":IRC " + code + " " + dest + " :" + message + "\n";
	if (send(msg.getAuthor()->getId(), temp.c_str(), temp.size(), 0) == -1)
		std::cerr << "Send error" << std::endl;
}

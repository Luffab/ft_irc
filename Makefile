NAME	=	ircserv

SRCS	=	srcs/main.cpp \
			srcs/Server.cpp \
			srcs/Message.cpp \
			srcs/User.cpp \
			srcs/Socket.cpp \
			srcs/Channel.cpp \
			srcs/commands/pass_cmd.cpp \
			srcs/commands/nick_cmd.cpp \
			srcs/commands/user_cmd.cpp \
			srcs/commands/join_cmd.cpp \
			srcs/commands/privmsg_cmd.cpp \
			srcs/commands/kill_cmd.cpp \
			srcs/commands/oper_cmd.cpp \
			srcs/commands/notice_cmd.cpp \
			srcs/commands/mode_cmd.cpp \
			srcs/commands/bot_cmd.cpp \
			srcs/commands/topic_cmd.cpp \
			srcs/commands/invite_cmd.cpp \
			srcs/commands/part_cmd.cpp \
			srcs/commands/kick_cmd.cpp \

HEADERS =	Server.hpp

INC		=   ./includes
CC		=	c++
CFLAGS	=	-Wall -Wextra -Werror -std=c++98
OBJS	=	$(SRCS:.cpp=.o)

LFLAGS  =   -I $(INC)

%.o:		%.cpp $(INC)/$(HEADERS)
	$(CC) $(CFLAGS) $(LFLAGS) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: all fclean clean re
GREEN		:=\033[0;32m
RED			:=\033[0;31m
BLUE		:=\033[0;34m
END			:=\033[0m

NAME		:= ircserv
CXXFLAGS		:= -Wextra -Wall -Werror -std=c++98
CC			:= c++

SRC_FOLDER	:= IrcServer/src
INCL		:= IrcServer/include
SRC			:= $(SRC_FOLDER)/main.cpp \
				$(SRC_FOLDER)/Socket.cpp \
				$(SRC_FOLDER)/Client.cpp \
				$(SRC_FOLDER)/Server.cpp \
				$(SRC_FOLDER)/Channel.cpp \
				$(SRC_FOLDER)/ChannelModule.cpp \
				$(SRC_FOLDER)/CommandModule.cpp \
				$(SRC_FOLDER)/CommandModule_Parser.cpp \
				$(SRC_FOLDER)/CommandModule_Mode.cpp \
				$(SRC_FOLDER)/CommandModule_ChannelCmds.cpp \
				$(SRC_FOLDER)/AuthentificationModule.cpp \

OBJ			:= $(SRC:.cpp=.o)

all: $(NAME)

bonus:
	make -C bot/

$(NAME): $(OBJ)
	@echo "$(GREEN)< Ircserv >$(END)"
	$(CC) $(CXXFLAGS) -I$(INCL) -g $(OBJ) -o $(NAME)

.c.o:
	$(CC) -g $(CXXFLAGS) -I$(INCL) -c $< -o $@

clean:
	rm -f $(OBJ)
	make clean -C bot/

fclean: clean
	rm -f $(NAME)
	make fclean -C bot/

re: clean all

.PHONY: bonus, all, clean, fclean, re

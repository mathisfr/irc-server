GREEN		:=\033[0;32m
RED			:=\033[0;31m
BLUE		:=\033[0;34m
END			:=\033[0m

NAME		:= bot
CXXFLAGS	:= -Wextra -Wall -Werror -std=c++98
CC			:= c++

SRC_FOLDER	:= src
INCL		:= include
SRC			:= $(SRC_FOLDER)/Bot.cpp \
				$(SRC_FOLDER)/main.cpp \
				$(SRC_FOLDER)/Parser.cpp \

OBJ			:= $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(GREEN)< Bot >$(END)"
	$(CC) $(CXXFLAGS) -I$(INCL) -g $(OBJ) -o $(NAME)

.c.o:
	$(CC) -g $(CXXFLAGS) -I$(INCL) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: clean all

.PHONY: all, clean, fclean, re

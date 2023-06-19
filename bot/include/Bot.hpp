#ifndef BOT_HPP
# define BOT_HPP
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <netdb.h>
#include <poll.h>
#include <fcntl.h>
#include <vector>
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr* ptrsockaddr;
typedef enum botState {JOINING_SERVER, JOINING_CHANNEL, IN_CHANNEL} botState;
class Bot
{
public:
	Bot(std::string user, std::string nick, std::string pass, std::string hostname, int port);
	~Bot();

	int	getFd(void);

	void	sendMessage(std::string message);
	std::vector<std::string> getMessages();
	void	start(void);
	void	waitForInput(void);
private:
	Bot();

	int			_fd;
	bool		_on;
	struct	pollfd	_ufd[1];
	std::string	_nick;
	std::string	_user;
	std::string	_messageBuffer;
	void answerToMessage(std::string message);
	botState _botState;

	bool	init(std::string hostname, int port);
	void	login(std::string);
	void	shutdown();
	sockaddr_in _addr;
};

#endif

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <sys/socket.h>
#include <sys/types.h>
# include <unistd.h>
# include <fcntl.h>
# include <set>
# include "../include/rpl_defines.hpp"

class Channel;
class Server;

class Client
{
public:
	Client(int socket, Server& server);
	~Client();

	bool	operator==(const Client &cl) const;
	bool	operator!=(const Client &cl) const;

	int						getFD() const;
	std::string				getNickname() const;
	std::string				getUsername() const;
	std::string 			getRealname() const;
	std::string				getID() const;
	bool					isRegistered() const;
	bool					hasCheckedPassword() const;
	std::vector<Channel*>	&getChannels();

	void		setAsRegistered();
	void		setAsPasswordChecked();
	void		changeNickname(std::string name);
	void		changeUsername(std::string name);
	void		changeRealname(std::string name);

	std::vector<std::string>	getMessages();
	void						sendMessage(std::string message);
	void						sendMessageAndToRelated(std::string message);

private:
	Client();

	std::vector<Channel*> _channels;
	int			_fd;
	std::string	_nickname;
	std::string	_username;
	std::string _realname;
	std::string _messageBuffer;
	bool		_operator;
	bool		_registered;
	bool		_passwordCheck;
	Server&		_server;
};

#endif

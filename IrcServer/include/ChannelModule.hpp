#ifndef CHANNELMODULE_CPP
# define CHANNELMODULE_CPP
# include <iostream>
# include "../include/Client.hpp"
# include "../include/Channel.hpp"
# include "map"
# include "vector"

class Server;

class ChannelModule
{
public:
	ChannelModule(Server& server);
	~ChannelModule();
	void								removeChannel(std::string name);
	std::string							joinChannel(std::string	name, Client &op, std::string key);
	std::map<std::string, Channel*>&	getChannels(void);
	Channel*							isChannel(std::string	name);
	void								removeAllInvite(Client &client);
private:
	std::map<std::string, Channel*> _channels;
	Server&		_server;
	void		addChannel(std::string	name, Client &op);
	const unsigned int	_limit;
};

#endif

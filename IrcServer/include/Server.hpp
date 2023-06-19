#ifndef __SERVER_H__
#define __SERVER_H__

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <poll.h>
#include <vector>
#include <map>
#include "../include/Socket.hpp"
#include "../include/Client.hpp"
#include "../include/Channel.hpp"
#include "../include/AuthentificationModule.hpp"
#include "../include/CommandModule.hpp"
#include "../include/ChannelModule.hpp"
#include "../include/rpl_defines.hpp"

typedef struct pollfd pollfd;


class Server
{
public:
	Server(int port, std::string password);
	~Server();

	static Server instance;

	bool isOn() const;
	void waitForInput();
	void addClient();
	void getMessageFrom(Client &sender);
	void removeClient(Client &client);
	Client	*isClient(std::string	name);
	void shutdown();
	void errorShutdown(std::string errorMsg);

	static std::string	ip;

	CommandModule & getCmdModule();
	ChannelModule &	getChannelModule();
	AuthentificationModule &	getAuthModule();
private:
	//Constructor and operator
	Server();
	Server(const Server& b);
	Server& operator =(const Server& b);

	//Modules
	AuthentificationModule _authentificationModule;
	CommandModule	_commandModule;
	ChannelModule	_channelModule;

	//Private attributes
	std::vector<Client*> _clients;
	std::vector<pollfd> _fdList;
	Socket _connectionSocket;
	int _port;
	int _nbOfClient;
	const unsigned int _limit;
	std::string _password;
	bool _isOn;
};
#endif // __SERVER_H__

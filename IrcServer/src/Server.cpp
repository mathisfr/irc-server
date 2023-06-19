#include "../include/Server.hpp"

std::string Server::ip = "";

Server::Server(int port, std::string password) :	_authentificationModule(*this, password),
													_commandModule(*this),
													_channelModule(*this),
													_connectionSocket(port),
													_limit(5)
{
	this->_port = port;
	this->_password = password;
	this->_nbOfClient = 0;
	this->_isOn = true;

	pollfd tmp;
	tmp.fd = _connectionSocket.getFD();
	ip = _connectionSocket.getIp();
	tmp.events = POLLIN;
	_fdList.push_back(tmp);
}

Server::~Server()
{
	shutdown();
}

bool Server::isOn() const
{
	return _isOn;
}

void Server::waitForInput()
{
	//Listening to all sockets
	int update_count = poll(&(_fdList[0]), _nbOfClient + 1, -1);

	if (update_count == -1)
		errorShutdown("Poll: error");

	//If there is a connection, we add the client
	if(_fdList[0].revents & POLLIN)
		addClient();

	//Checking other sockets for pending events
	for(int i = 1; i < _nbOfClient + 1; i++)
	{
		if (_fdList[i].revents & POLLIN)
			getMessageFrom(*_clients[i - 1]);
		if (_fdList[i].revents & POLLHUP)
			removeClient(*_clients[i - 1]);
	}
}

void Server::addClient()
{
	std::cout << "Adding client" << std::endl;
	Client *client = new Client (_connectionSocket.ircaccept(), *this);

	if (!client)
		errorShutdown("Memory allocation error");

	if (_clients.size() >= _limit)
	{
		std::string nick = client->getNickname();
		std::cout << "Client " << nick << " can't be add" << std::endl;
		client->sendMessage(ERR_519_TOOMANYUSERS);
		delete client;
		return;
	}
	_clients.push_back(client);

	pollfd tmp;
	tmp.fd = client->getFD();
	tmp.events = POLLIN;
	_fdList.push_back(tmp);

	_nbOfClient++;
}

void Server::getMessageFrom(Client &sender)
{
	if (sender.isRegistered() == false)
		_authentificationModule.handleAuthenticateProcess(sender);
	else
		_commandModule.handleCommands(sender);
}

void Server::removeClient(Client &client)
{
	for(int i = 0; i < _nbOfClient; i++)
	{
		if(*_clients[i] == client)
		{
			_channelModule.removeAllInvite(client);
			delete _clients[i];
			_clients.erase(_clients.begin() + i);
			_nbOfClient--;
			std::cout << "Removing client" << std::endl;
			_fdList.erase(_fdList.begin() + i + 1);
			break;
		}
	}
}

Client* Server::isClient(std::string	name)
{
	std::vector<Client*>::iterator	it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if((*it)->getNickname() == name)
			return (*it);
	}
	return NULL;
}

void Server::shutdown()
{
	if (!this->_isOn)
		return;

	for(int i = 0; i < _nbOfClient; i++)
		delete _clients[i];
	_clients.clear();

	_connectionSocket.shutdown();

	this->_isOn = false;
}

void Server::errorShutdown(std::string errorMsg)
{
	std::cerr << errorMsg << std::endl;
	shutdown();
	throw std::exception();
}

CommandModule & Server::getCmdModule()
{
	return _commandModule;
}

ChannelModule &	Server::getChannelModule()
{
	return _channelModule;
}

/*AuthentificationModule &	getAuthModule()
{
	return _authentificationModule;
}*/

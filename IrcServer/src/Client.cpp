#include "../include/Client.hpp"
#include "../include/Channel.hpp"
#include "../include/Server.hpp"

Client::Client(int socket, Server& server) : _fd(socket),
								_nickname(""),
								_username(""),
								_realname(""),
								_operator(false),
								_registered(false),
								_passwordCheck(false),
								_server(server)
{

	fcntl(_fd, F_SETFL, O_NONBLOCK);
}

int	Client::getFD() const
{
	return _fd;
}

std::string Client::getNickname() const
{
	return _nickname;
}

std::string Client::getUsername() const
{
	return _username;
}

std::string Client::getRealname() const
{
	return _realname;
}

std::string Client::getID() const
{
	return _nickname + "!" + _username + "@";
}

std::vector<Channel*> & Client::getChannels()
{
	return _channels;
}

bool Client::isRegistered() const
{
	return _registered;
}

bool Client::hasCheckedPassword() const
{
	return _passwordCheck;
}

void Client::setAsRegistered()
{
	_registered = true;
}

void Client::setAsPasswordChecked()
{
	_passwordCheck = true;
}

std::vector<std::string> Client::getMessages()
{
	std::string messageBuffer = _messageBuffer;
	std::vector<std::string> output;

	char buf[512];
	int size = recv(this->_fd, buf, sizeof(buf) - 1, 0);
	if (size == -1)
		_server.errorShutdown("Recv: error");

	buf[size] = '\0';

	for(int i = 0; i < size; i++)
	{
		if ((buf[i] == '\r' || buf[i] == '\n' || buf[i] == 10) && !messageBuffer.empty())
		{
			output.push_back(messageBuffer);
			messageBuffer = "";
			while(buf[i] == '\r' || buf[i] == '\n' || buf[i] == 10)
				i++;
			i--;
		}
		else
			messageBuffer += buf[i];
	}
	_messageBuffer = messageBuffer;
	return output;
}

void Client::sendMessage(std::string message)
{
	if (message[message.size() - 1] != '\n')
	{
		message += "\r\n";
	}
	int rtn_value = send(this->_fd, message.c_str(), message.size(), 0);
	if (rtn_value == -1)
		std::cout << "error sending message to " << _nickname << std::endl;
}

void Client::sendMessageAndToRelated(std::string message)
{
	std::set<Client*> neighbours;

	neighbours.insert(this);

	for(unsigned int i = 0; i < _channels.size(); i++)
		neighbours.insert(_channels[i]->getClients().begin(), _channels[i]->getClients().end());

	for(std::set<Client*>::const_iterator it = neighbours.begin(); it != neighbours.end(); it++)
		(*it)->sendMessage(message);
}

void Client::changeNickname(std::string name)
{
	_nickname = name;
}

void Client::changeUsername(std::string name)
{
	_username = name;
}

void Client::changeRealname(std::string name)
{
	_realname = name;
}

bool	Client::operator==(const Client &cl) const
{
	return (_fd == cl._fd);
}

bool	Client::operator!=(const Client &cl) const
{
	return !(*this == cl);
}

Client::~Client()
{
	close(_fd);

	for(unsigned int i = 0; i < _channels.size(); i++)
	{
		_channels[i]->removeClient(*this);
		_channels[i]->removeInvite(*this);
	}
}

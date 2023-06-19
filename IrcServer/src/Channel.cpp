#include "../include/Channel.hpp"
#include "../include/Server.hpp"

Channel::Channel(std::string name, Client & op, ChannelModule& channelModule) : _channelModule(channelModule), _maxTopicLenght(25)
{
	std::cout << "Creating channel : " << name << '\n';
	_mode.inviteonly = false;
	_mode.topic = false;
	_mode.key = false;
	_mode.userlimit = false;
	std::time_t now = std::time(NULL);
	_date = std::to_string(static_cast<long>(now));
	_name = name;
	addOp(op);
	addClient(op);
}

Channel::~Channel()
{
	std::vector<Client *>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		removeClient(*(*it));
	}
}

void Channel::addClient(Client & client)
{
	client.getChannels().push_back(this);
	_clients.push_back(&client);
	removeInvite(client);
}

void Channel::addOp(Client & client)
{
	if (!isOp(client))
		_ops.push_back(&client);
}

void Channel::addInvite(Client & client)
{
	if (!isInvite(client))
		_invites.push_back(&client);
}

void Channel::removeClient(Client & client)
{
	std::vector<Client *>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (*(*it) == client)
		{
			std::vector<Channel*>::iterator	it_c;
			for (it_c = (*it)->getChannels().begin(); it_c != (*it)->getChannels().end(); it_c++)
			{
				if ((*it_c)->getName() == this->getName())
				{
					 (*it)->getChannels().erase(it_c);
					 break;
				}
			}
			if (isOp(client))
				removeOp(*(*it));
			_clients.erase(it);
			if (_clients.empty())
				_channelModule.removeChannel(_name);
			return ;
		}
	}
}

std::string	Channel::getMode(void)
{
	std::string mymodes = "";

	if (_mode.inviteonly)
		mymodes += 'i';
	if (_mode.topic)
		mymodes += 't';
	if (_mode.key)
		mymodes += 'k';
	if (_mode.userlimit)
		mymodes += 'l';

	return mymodes;
}

void Channel::removeOp(Client &client)
{
	std::vector<Client *>::iterator it;
	for (it = _ops.begin(); it != _ops.end(); it++)
	{
		if (*(*it) == client)
		{
			_ops.erase(it);
			return ;
		}
	}
}

void Channel::removeInvite(Client & client)
{
	std::vector<Client *>::iterator it;
	for (it = _invites.begin(); it != _invites.end(); it++)
	{
		if (*(*it) == client)
		{
			_invites.erase(it);
			return ;
		}
	}
}

std::string	Channel::getUsersInChannel(void)
{
	std::vector<Client *>::iterator it;
	std::string	list_clients;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (list_clients.size() < 1)
		{
			if (isOp(*(*it)))
				list_clients += '@';
			list_clients += (*it)->getNickname();
		}
		else
		{
			list_clients += " ";
			if (isOp(*(*it)))
				list_clients += '@';
			list_clients += (*it)->getNickname();
		}
	}
	return list_clients;
}

std::vector<Client*> & Channel::getInvites()
{
	return _invites;
}

std::string Channel::getName(void) const
{
	return (_name);
}

void Channel::setTopic(std::string topic)
{
	_topic = topic;
}

std::string Channel::getTopic(void) const
{
	return (_topic);
}

std::string Channel::getDate()
{
	return (_date);
}

bool Channel::isInvite(Client & client)
{
	std::vector<Client *>::iterator it;
	for (it = _invites.begin(); it != _invites.end(); it++)
	{
		if (*(*it) == client)
			return	(true);
	}
	return (false);
}

std::vector<Client*> & Channel::getClients(void)
{
	return _clients;
}

bool Channel::isInChannel(Client& client)
{
	std::vector<Client *>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (*(*it) == client)
			return	(true);
	}
	return (false);
}

bool Channel::isOp(Client& client)
{
	std::vector<Client *>::iterator it;
	for (it = _ops.begin(); it != _ops.end(); it++)
	{
		if (*(*it) == client)
			return	(true);
	}
	return (false);
}

void Channel::sendMsgToAllUsers(std::string message, Client& sender)
{
	for(int i = 0; i < static_cast<int>(_clients.size()); i++)
		if (*_clients[i] != sender)
			_clients[i]->sendMessage(message);
}

void Channel::sendMsgToOperators(std::string message, Client& sender)
{
	for(int i = 0; i < static_cast<int>(_ops.size()); i++)
		if (*_ops[i] != sender)
			_ops[i]->sendMessage(message);
}

bool Channel::getModeInvite(void) const
{
	return _mode.inviteonly;
}

bool Channel::getModeTopic(void) const
{
	return _mode.topic;
}

bool Channel::getModeKey(void) const
{
	return _mode.key;
}

bool Channel::getModeUserLimit(void) const
{
	return _mode.userlimit;
}

size_t Channel::getUserLimit(void) const
{
	return _userlimit;
}

std::string Channel::getPassword(void) const
{
	return _password;
}

void Channel::setModeInvite(bool state)
{
	_mode.inviteonly = state;
}

void Channel::setModeTopic(bool state)
{
	_mode.topic = state;
}

void Channel::setModeKey(bool state)
{
	_mode.key = state;
}

void Channel::setModeUserLimit(bool state)
{
	_mode.userlimit = state;
}

void Channel::setPassword(std::string pass)
{
	_password = pass;
}

void Channel::setUserLimit(size_t	us)
{
	_userlimit = us;
}

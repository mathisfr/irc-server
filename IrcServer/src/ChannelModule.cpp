#include "../include/ChannelModule.hpp"
#include "../include/Server.hpp"

ChannelModule::ChannelModule(Server& server) : _server(server), _limit(3)
{
}

ChannelModule::~ChannelModule()
{
	for(std::map<std::string, Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); it++)
		delete it->second;
	_channels.clear();
}

void ChannelModule::addChannel(std::string name, Client &client)
{
	std::string channelName = name;
	if (_channels.size() >= _limit)
	{
		client.sendMessage(ERR_405_TOMANYCHAN);
		return;
	}
	Channel	*channel = new Channel(name, client, *this);
	if (!channel)
		_server.errorShutdown("Memory allocation error");

	_channels[name] = channel;
}

std::map<std::string, Channel*>& ChannelModule::getChannels(void)
{
	return (_channels);
}

Channel* ChannelModule::isChannel(std::string name)
{
	std::map<std::string, Channel*>::const_iterator it = _channels.find(name);

	if (it != _channels.end())
		return it->second;
	return NULL;
}

void ChannelModule::removeAllInvite(Client &client)
{
	for(std::map<std::string, Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		it->second->removeInvite(client);
	}
}
void ChannelModule::removeChannel(std::string name)
{
	std::map<std::string, Channel*>::const_iterator it = _channels.find(name);

	if (it != _channels.end())
	{
		if ((*it).second)
			delete (*it).second;
		_channels.erase(it);
		return ;
	}
}

std::string ChannelModule::joinChannel(std::string topic, Client &client, std::string key)
{
	Channel *channel = isChannel(topic);
	if (channel)
	{
		std::string channelName = channel->getName();
		if (channel->isInvite(client))
		{
			channel->addClient(client);
			return "";
		}

		if (channel->getModeInvite())
			return ERR_473_INVITEONLYCHAN;

		if (channel->getModeKey() && channel->getPassword() != key)
			return ERR_475_BADCHANNELKEY;

		if (channel->getModeUserLimit() && channel->getClients().size() >= channel->getUserLimit())
			return ERR_471_CHANNELISFULL;

		channel->addClient(client);
	}
	else
	{
		std::string channelName = topic;

		if (topic[0] != '#')
			return ERR_403_NOSUCHCHANNEL;

		addChannel(topic, client);
	}

	return "";
}

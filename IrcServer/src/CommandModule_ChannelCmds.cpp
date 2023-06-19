#include "../include/CommandModule.hpp"
#include "../include/Server.hpp"
#include "../include/Channel.hpp"

void CommandModule::handleTopicCommand(Client &client, std::string const &msg)
{
	std::string channelName = getArg(msg, 1);
	Channel *channel = _channelModule.isChannel(channelName);

	if (getArgsCount(msg) < 2)
		return client.sendMessage(ERR_461_NEEDMOREPARAMS);
	if (!channel)
		return client.sendMessage(ERR_403_NOSUCHCHANNEL);
	if (getArgsCount(msg) == 2)
		return client.sendMessage(RPL_332_TOPIC);

	std::string topic = getLastArg(msg, 2);

	if (!channel->isInChannel(client))
		return client.sendMessage(ERR_442_NOTONCHANNEL);
	if (channel->getModeTopic() && !channel->isOp(client))
		return client.sendMessage(ERR_482_CHANOPRIVSNEEDED);

	channel->setTopic(topic);
	client.sendMessage(LOG_TOPICCHANGED);
	channel->sendMsgToAllUsers(LOG_TOPICCHANGED, client);
}

void CommandModule::handleKickCommand(Client &client, std::string const &msg)
{
	if (getArgsCount(msg) < 3)
		return client.sendMessage(ERR_461_NEEDMOREPARAMS);

	std::string channelName = getArg(msg, 1);
	Channel *channel = _channelModule.isChannel(channelName);
	std::string nick = getArg(msg, 2);
	Client *user = _server.isClient(nick);
	std::string reason;

	if (getArgsCount(msg) == 3)
		reason = client.getNickname();
	else
		reason = getLastArg(msg, 3);

	if (!channel)
		return client.sendMessage(ERR_403_NOSUCHCHANNEL);
	if (!user)
		return client.sendMessage(ERR_401_NOSUCHNICK);
	if (!channel->isInChannel(client))
		return client.sendMessage(ERR_442_NOTONCHANNEL);
	if (!channel->isInChannel(*user))
		return client.sendMessage(ERR_441_USERNOTINCHANNEL);
	if (!channel->isOp(client))
		return client.sendMessage(ERR_482_CHANOPRIVSNEEDED);

	channel->sendMsgToAllUsers(LOG_USERKICKED, client);
	client.sendMessage(LOG_USERKICKED);

	channel->removeClient(*user);
}

void CommandModule::handleInviteCommand(Client &client, std::string const &msg)
{
	if (getArgsCount(msg) < 3)
		return client.sendMessage(ERR_461_NEEDMOREPARAMS);
		
	std::string nick = getArg(msg, 1);
	std::string channelName = getArg(msg, 2);

	Client *user = _server.isClient(nick);
	Channel *channel = _channelModule.isChannel(channelName);

	if (!user)
		return client.sendMessage(ERR_401_NOSUCHNICK);
	if (!channel)
		return client.sendMessage(ERR_403_NOSUCHCHANNEL);
	if (!channel->isInChannel(client))
		return client.sendMessage(ERR_442_NOTONCHANNEL);
	if (channel->isInChannel(*user))
		return client.sendMessage(ERR_443_USERONCHANNEL);
	if (!channel->isOp(client))
		return client.sendMessage(ERR_482_CHANOPRIVSNEEDED);
		
	client.sendMessage(RPL_341_INVITING);
	user->sendMessage(LOG_USERINVITE);

	channel->addInvite(*user);
}

void CommandModule::handleJoinCommand(Client &client, std::string const &msg)
{
	if (getArgsCount(msg) < 2)
		return client.sendMessage(ERR_461_NEEDMOREPARAMS);

	std::vector<std::string> channelList = splitArg(getArg(msg, 1));

	for(unsigned int i = 0; i < channelList.size(); i++)
	{
		std::string channelName = channelList[i];
		std::string channelKey = getArg(msg, i + 2);
		Channel *channel = _channelModule.isChannel(channelName);

		if (channel && channel->isInChannel(client))
			continue;

		std::string errorMessage = _channelModule.joinChannel(channelName, client, channelKey);

		if (!channel)
			channel = _channelModule.isChannel(channelName);
		if (!channel || !channel->isInChannel(client))
		{
			client.sendMessage(errorMessage);
			continue;
		}

		client.sendMessage(LOG_CHANNELJOINED);
		channel->sendMsgToAllUsers(LOG_CHANNELJOINED, client);

		if(channel->getTopic().empty())
			client.sendMessage(RPL_331_NOTOPIC);
		else
			client.sendMessage(RPL_332_TOPIC);

		client.sendMessage(RPL_353_NAMREPLY);
		client.sendMessage(RPL_366_ENDOFNAMES);
	}
}

void CommandModule::handlePartCommand(Client &client, std::string const &msg)
{
	if (getArgsCount(msg) < 2)
		return client.sendMessage(ERR_461_NEEDMOREPARAMS);

	std::string channelName = getArg(msg, 1);
	Channel *channel = _channelModule.isChannel(channelName);

	if(!channel)
		return client.sendMessage(ERR_403_NOSUCHCHANNEL);
	if (!channel->isInChannel(client))
		return client.sendMessage(ERR_442_NOTONCHANNEL);

	client.sendMessage(LOG_CHANNELPARTED);
	channel->sendMsgToAllUsers(LOG_CHANNELPARTED, client);

	channel->removeClient(client);
}
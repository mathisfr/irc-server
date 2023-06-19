#include "../include/CommandModule.hpp"
#include "../include/Server.hpp"
#include "../include/Channel.hpp"

CommandModule::CommandModule(Server &server) : _server(server), _channelModule(server.getChannelModule())
{
	_cmdMap["CAP LS"] = 	&CommandModule::handleCapCommand;
	_cmdMap["WHO"] = 		&CommandModule::handleWhoCommand;
	_cmdMap["MODE"] = 		&CommandModule::handleModeCommand;
	_cmdMap["TOPIC"] = 		&CommandModule::handleTopicCommand;
	_cmdMap["KICK"] = 		&CommandModule::handleKickCommand;
	_cmdMap["INVITE"] = 	&CommandModule::handleInviteCommand;
	_cmdMap["JOIN"] = 		&CommandModule::handleJoinCommand;
	_cmdMap["QUIT"] = 		&CommandModule::handleQuitCommand;
	_cmdMap["PRIVMSG"] =	&CommandModule::handlePrivmsgCommand;
	_cmdMap["ISON"] =		&CommandModule::handleIsonCommand;
	_cmdMap["NICK"] = 		&CommandModule::handleNickCommand;
	_cmdMap["USER"] =		&CommandModule::handleUserCommand;
	_cmdMap["PING"] =		&CommandModule::handlePingCommand;
	_cmdMap["PART"] =		&CommandModule::handlePartCommand;
}

CommandModule::~CommandModule()
{

}

void CommandModule::handleCommands(Client &client)
{
	std::vector<std::string> messages = client.getMessages();
	std::string first_arg;
	std::map<std::string, void (CommandModule::*)(Client &, std::string const&)>::const_iterator it;

	for(int i = 0; i < static_cast<int>(messages.size()); i++)
	{
		std::cout << "message : " << messages[i] << std::endl;
		first_arg = getFirstArgFromCommand(messages[i]);
		if (first_arg.empty())
			continue;
		it = _cmdMap.find(first_arg);

		if(it != _cmdMap.end())
			(this->*(it->second))(client, messages[i]);
		else
			client.sendMessage(ERR_421_UNKNOWNCOMMAND);
	}
}

void CommandModule::handleCapCommand(Client &client, std::string const &msg)
{
	static_cast<void>(msg);
	client.sendMessage("CAP * LS :\r\n");
}

void CommandModule::handleNickCommand(Client &client, std::string const &msg)
{
	if (getArgsCount(msg) < 2)
		return client.sendMessage(ERR_461_NEEDMOREPARAMS);

	std::string newNick = getArg(msg, 1);

	if (!isValidNickname(newNick))
		return client.sendMessage(ERR_432_ERRONEUSNICKNAME);
	if (_server.isClient(newNick))
		return client.sendMessage(ERR_433_NICKNAMEINUSE);

	client.sendMessageAndToRelated(LOG_NICKNAMECHANGE);
	client.changeNickname(getArg(msg, 1));
}

void CommandModule::handleUserCommand(Client &client, std::string const &msg)
{
	if (getArgsCount(msg) < 5)
		return client.sendMessage(ERR_461_NEEDMOREPARAMS);
	if (client.isRegistered())
		return client.sendMessage(ERR_462_ALREADYREGISTERED);

	client.changeUsername(getArg(msg, 1));
	client.changeRealname(getLastArg(msg, 4));
}

void CommandModule::handleWhoCommand(Client &client, std::string const &msg)
{
	if (getArgsCount(msg) < 2)
		return client.sendMessage(ERR_461_NEEDMOREPARAMS);

	std::string channelName = getArg(msg, 1);
	Channel *channel = _channelModule.isChannel(channelName);

	if(channel && channel->isInChannel(client))
	{
		std::vector<Client*> clientList = channel->getClients();

		for(int i = 0; i < static_cast<int>(clientList.size()); i++)
			client.sendMessage(RPL_352_WHOREPLY);
	}

	client.sendMessage(RPL_315_ENDOFWHO);
}

void CommandModule::handleQuitCommand(Client &client, std::string const &msg)
{
	static_cast<void>(msg);
	client.sendMessageAndToRelated(LOG_USERQUIT);
	_server.removeClient(client);
}

void CommandModule::handlePrivmsgCommand(Client &client, std::string const &msg)
{
	if (getArgsCount(msg) < 3)
		return client.sendMessage(ERR_461_NEEDMOREPARAMS);

	std::vector<std::string> targetList = splitArg(getArg(msg, 1));
	std::string sendMsg = getLastArg(msg, 2);
	bool opOnly = false;

	for(unsigned int i = 0; i < targetList.size(); i++)
	{
		if (targetList[i].size() > 2 && targetList[i][0] == '@' && targetList[i][1] == '#')
		{
			opOnly = true;
			targetList[i] = targetList[i].substr(1);
		}
		else
			opOnly = false;

		if (targetList[i][0] == '#')
			privMsgToChannel(targetList[i], client, opOnly, sendMsg);
		else
			privMsgToUser(targetList[i], client, sendMsg);
	}
}

void CommandModule::privMsgToChannel(std::string target, Client& client, bool opOnly, std::string sendMsg)
{
	std::string channelName = target;
	Channel *channel = _channelModule.isChannel(channelName);

	if(!channel)
		return client.sendMessage(ERR_403_NOSUCHCHANNEL);
	if(!channel->isInChannel(client))
		return client.sendMessage(ERR_442_NOTONCHANNEL);

	if(opOnly)
		channel->sendMsgToOperators(LOG_PRIVMSG, client);
	else
		channel->sendMsgToAllUsers(LOG_PRIVMSG, client);
}

void CommandModule::privMsgToUser(std::string target, Client &client, std::string sendMsg)
{
	std::string nick = target;
	Client *targetClient = _server.isClient(nick);

	if (!targetClient)
		return client.sendMessage(ERR_401_NOSUCHNICK);

	targetClient->sendMessage(LOG_PRIVMSG);
}

void CommandModule::handleIsonCommand(Client &client, std::string const &msg)
{
	if (getArgsCount(msg) < 2)
		return client.sendMessage(ERR_461_NEEDMOREPARAMS);

	std::string ison = getLastArg(msg, 1);
	size_t	i = 0;
	std::string names;
	while (i < getArgsCount(ison))
	{
		std::string arg = getArg(ison, i);
		if (_server.isClient(arg))
		{
			if (i == 0)
				names += arg;
			else
				names += " " + arg;
		}
		i++;
	}
	client.sendMessage(RPL_303_TOPICWHOTIME);
}

void CommandModule::handlePingCommand(Client &client, std::string const &msg)
{
	if (getArgsCount(msg) < 2)
		return client.sendMessage(ERR_461_NEEDMOREPARAMS);

	client.sendMessage(LOG_PONGANSWER);
}

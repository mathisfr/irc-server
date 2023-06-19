#ifndef __COMMANDMODULE_H__
#define __COMMANDMODULE_H__

#include "../include/Client.hpp"
#include "../include/rpl_defines.hpp"
#include "../include/ChannelModule.hpp"
#include <map>
#include <string>
#include <vector>
#include <ctime>

class Server;

class CommandModule
{
public:
	CommandModule(Server &server);
	~CommandModule();

	void handleCommands(Client &client);

	void handleCapCommand(Client &client, std::string const &msg);
	void handleNickCommand(Client &client, std::string const &msg);
	void handleUserCommand(Client &client, std::string const &msg);

	std::string getFirstArgFromCommand(std::string const &msg);
	unsigned int 				getArgsCount(std::string const &msg);
	std::string					getArg(std::string const &msg, unsigned int arg_number);

private:
	CommandModule();
	CommandModule(const CommandModule &b);

	CommandModule& operator =(CommandModule const &b);

	void 						handleWhoCommand(Client &client, std::string const &msg);
	void 						handleModeCommand(Client &client, std::string const &msg);
	void 						handleTopicCommand(Client &client, std::string const &msg);
	void 						handleKickCommand(Client &client, std::string const &msg);
	void 						handleInviteCommand(Client &client, std::string const &msg);
	void 						handleJoinCommand(Client &client, std::string const &msg);
	void						handlePartCommand(Client &client, std::string const &msg);
	void 						handleQuitCommand(Client &client, std::string const &msg);
	void 						handlePrivmsgCommand(Client &client, std::string const &msg);
	void						handleIsonCommand(Client &client, std::string const &msg);
	void						handlePingCommand(Client &client, std::string const &msg);
	
	void 						privMsgToChannel(std::string target, Client& client, bool opOnly, std::string sendMsg);
	void						privMsgToUser(std::string target, Client &client, std::string sendMsg);
	
	std::string					getLastArg(std::string const &msg, unsigned int arg_number);
	std::vector<std::string>	splitArg(std::string const &arg);
	bool						isValidNickname(std::string const &nick) const;

	Server& _server;
	ChannelModule& _channelModule;
	std::map<std::string, void (CommandModule::*)(Client &, std::string const &)> _cmdMap;

};
#endif // __COMMANDMODULE_H__

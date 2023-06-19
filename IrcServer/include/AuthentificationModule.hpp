#ifndef __AUTHENTIFICATIONMODULE_H__
#define __AUTHENTIFICATIONMODULE_H__

#include "../include/Client.hpp"
#include "../include/rpl_defines.hpp"
#include "../include/CommandModule.hpp"
#include <map>
#include <string>
#include <vector>

class Server;

class AuthentificationModule
{
public:
	AuthentificationModule(Server &server, std::string const &password);
	~AuthentificationModule();

	void handleAuthenticateProcess(Client &client);

private:
	AuthentificationModule();
	AuthentificationModule(AuthentificationModule const &b);

	AuthentificationModule& operator =(AuthentificationModule const &b);

	std::string getFirstArgFromCommand(std::string const &msg) const;

	bool checkPassword(Client &client, std::string const &msg);
	bool endAuthentification(Client &client, std::string const &msg);
	bool handleCapCommand(Client &client, std::string const &msg);
	bool handleNickCommand(Client &client, std::string const &msg);
	bool handleUserCommand(Client &client, std::string const &msg);


	Server &_server;
	CommandModule &_commandModule;
	std::string const &_password;
	std::map<std::string, bool (AuthentificationModule::*)(Client &, std::string const&)> _map;
};
#endif // __AUTHENTIFICATIONMODULE_H__

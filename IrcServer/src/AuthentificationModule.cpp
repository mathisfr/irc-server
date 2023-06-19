#include "../include/AuthentificationModule.hpp"
#include "../include/Server.hpp"

AuthentificationModule::AuthentificationModule(Server &server, std::string const &password) : _server(server),
																									_commandModule(server.getCmdModule()),
																									_password(password)
{
	_map["CAP LS"] = &AuthentificationModule::handleCapCommand;
	_map["NICK"] = &AuthentificationModule::handleNickCommand;
	_map["PASS"] = &AuthentificationModule::checkPassword;
	_map["CAP END"] = &AuthentificationModule::endAuthentification;
	_map["USER"] = &AuthentificationModule::handleUserCommand;
}

AuthentificationModule::~AuthentificationModule()
{

}

void AuthentificationModule::handleAuthenticateProcess(Client &client)
{
	std::vector<std::string> messages = client.getMessages();
	std::string first_arg;
	std::map<std::string, bool (AuthentificationModule::*)(Client &, std::string const&)>::const_iterator it;
	bool		is_kicked = false;

	for(int i = 0; i < static_cast<int>(messages.size()); i++)
	{
		std::cout << "message : " << messages[i] << std::endl;
		first_arg = _commandModule.getFirstArgFromCommand(messages[i]);
		it = _map.find(first_arg);

		if(it != _map.end())
			is_kicked = (this->*(it->second))(client, messages[i]);

		if (is_kicked)
			break;
	}
}

bool AuthentificationModule::checkPassword(Client &client, std::string const &msg)
{
	if (_commandModule.getArgsCount(msg) < 2)
		client.sendMessage(ERR_461_NEEDMOREPARAMS);
	else if (_commandModule.getArg(msg, 1) == _password)
		client.setAsPasswordChecked();
	else
	{
		client.sendMessage(ERR_464_PASSWDMISMATCH);
		_server.removeClient(client);
		return true;
	}
	return false;
}

bool AuthentificationModule::endAuthentification(Client &client, std::string const &useless)
{
	static_cast<void>(useless);
	if (client.hasCheckedPassword() && client.getNickname() != ""
		&& client.getRealname() != "" && client.getUsername() != "")
	{
		client.setAsRegistered();
		client.sendMessage(RPL_001_WELCOME);
		client.sendMessage(RPL_002_YOURHOST);
		client.sendMessage(RPL_003_CREATED);
		client.sendMessage(RPL_004_MYINFO);
		client.sendMessage(RPL_005_ISUPPORT);
	}
	else
	{
		if (client.getNickname() == "")
			client.sendMessage(ERR_433_NICKNAMEINUSE);
		else
			client.sendMessage(ERR_464_PASSWDMISMATCH);
		_server.removeClient(client);
		return true;
	}
	return false;
}

bool AuthentificationModule::handleCapCommand(Client &client, std::string const &msg)
{
	_commandModule.handleCapCommand(client, msg);
	return false;
}

bool AuthentificationModule::handleNickCommand(Client &client, std::string const &msg)
{
	_commandModule.handleNickCommand(client, msg);
	return false;
}

bool AuthentificationModule::handleUserCommand(Client &client, std::string const &msg)
{
	_commandModule.handleUserCommand(client, msg);
	return false;
}

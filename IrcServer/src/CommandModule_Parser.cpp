#include "../include/CommandModule.hpp"
#include "../include/Server.hpp"
#include "../include/Channel.hpp"

std::string CommandModule::getFirstArgFromCommand(std::string const &msg)
{
	std::string output;

	if(msg.substr(0, 6) == "CAP LS")
		return "CAP LS";
	if(msg.substr(0, 7) == "CAP END")
		return "CAP END";

	int i = 0;

	while (msg[i] == ' ')
		i++;

	for(; msg[i] && msg[i] != ' ' && msg[i] != '\r'; i++)
		output += std::toupper(msg[i]);

	return output;
}

unsigned int CommandModule::getArgsCount(std::string const &msg)
{
	std::stringstream	stream(msg);
	std::string			oneWord;
	unsigned int		count = 0;

	while(stream >> oneWord)
		count++;

	return count;
}

std::string	CommandModule::getArg(std::string const &msg, unsigned int arg_number)
{
	std::stringstream	stream(msg);
	std::string			oneWord;

	for(unsigned int i = 0; i <= arg_number; i++)
		stream >> oneWord;

	return oneWord;
}

std::string CommandModule::getLastArg(std::string const &msg, unsigned int arg_number)
{
	std::stringstream	stream(msg);
	std::string			oneWord;

	for(unsigned int i = 0; i < arg_number; i++)
		stream >> oneWord;

	getline(stream, oneWord);

	int i = 0;

	while(oneWord[i] == ' ' || oneWord[i] == ':')
		i++;

	return std::string(oneWord.substr(i, oneWord.size() - i));
}

std::vector<std::string> CommandModule::splitArg(std::string const &arg)
{
	std::vector<std::string> output;
	std::stringstream stream(arg);
	std::string item;

	while(getline(stream, item, ','))
		output.push_back(item);

	return output;
}

bool CommandModule::isValidNickname(std::string const &nick) const
{
	if (nick.empty())
		return false;
	if (nick[0] == '$' || nick[0] == ':' || nick[0] == '#')
		return false;

	for(int i = 0; i < static_cast<int>(nick.size()); i++)
	{
		if (nick[i] == ' ' || nick[i] == ',' || nick[i] == '*' || nick[i] == '?'
			|| nick[i] == '!' || nick[i] == '@' || nick[i] == '.')
			return false;
	}
	return true;
}

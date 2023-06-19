#include "../include/Parser.hpp"

Parser::Parser()
{
}

std::string Parser::getFirstArgFromCommand(std::string const &msg)
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
		output += msg[i];

	return output;
}

unsigned int Parser::getArgsCount(std::string const &msg)
{
	std::stringstream	stream(msg);
	std::string			oneWord;
	unsigned int		count = 0;

	while(stream >> oneWord)
		count++;

	return count;
}

std::string	Parser::getArg(std::string const &msg, unsigned int arg_number)
{
	std::stringstream	stream(msg);
	std::string			oneWord;

	for(unsigned int i = 0; i <= arg_number; i++)
		stream >> oneWord;

	return oneWord;
}

std::string Parser::getLastArg(std::string const &msg, unsigned int arg_number)
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

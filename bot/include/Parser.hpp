#include <iostream>
#include <string>
#include <sstream>

class Parser
{
public:
	virtual ~Parser() = 0;
	static unsigned int getArgsCount(std::string const &msg);
	static std::string	getArg(std::string const &msg, unsigned int arg_number);
	static std::string getLastArg(std::string const &msg, unsigned int arg_number);
	static std::string getFirstArgFromCommand(std::string const &msg);
private:
	Parser();
};

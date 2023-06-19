#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../include/Socket.hpp"
#include "../include/Server.hpp"

void checkPort(char *port);
void checkPass(char *pass);

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Wrong argument count !" << std::endl;
		std::cerr << "Usage : ./ircserv PORT PASSWORD" << std::endl;
		return 0;
	}

	checkPort(argv[1]);
	checkPass(argv[2]);

	signal(SIGPIPE, SIG_IGN);
	Server server(atoi(argv[1]), std::string(argv[2]));

	try
	{
		while(server.isOn())
			server.waitForInput();
	}
	catch (std::exception e)
	{
		std::cerr << "Server crashed\n";
	}
	return 0;
}

void checkPort(char *port)
{
	int i = 0;
	while (port[i])
	{
		if (!isnumber(port[i]))
		{
			std::cerr << "Port must be a number between 6665 and 6669" << std::endl;
			exit(0);
		}
		i++;
	}
	int portNumber = atoi(port);
	if (portNumber < 6665 || portNumber > 6669)
	{
		std::cerr << "Port must be a number between 6665 and 6669" << std::endl;
		exit(0);
	}
}

void checkPass(char *pass)
{
	for(int i = 0; pass[i]; i++)
	{
		if(isspace(pass[i]))
		{
			std::cerr << "Password must not contain any space\n";
			exit(0);
		}
	}

	if (!pass[0])
	{
		std::cerr << "Password must not be empty\n";
		exit(0);
	}
}

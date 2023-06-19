#include <iostream>
#include <string>
#include <vector>
#include "../include/Bot.hpp"

void checkPort(char *port);
void checkIP(std::string ip);
std::vector<std::string> split(std::string s, std::string delimiter);

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		std::cerr << "Wrong argument count !" << std::endl;
		std::cerr << "Usage : ./bot IP PORT PASSWORD" << std::endl;
		return 0;
	}

	checkIP(argv[1]);
	checkPort(argv[2]);


	Bot bot("BOT IRC", "MyBot", argv[3], argv[1], atoi(argv[2]));
	bot.start();

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

void checkIP(std::string ip)
{
	std::vector<std::string> ip_numbers = split(ip, ".");
	int value;

	if (ip_numbers.size() != 4)
	{
		std::cerr << "Ip format is wrong ! Must be : x.x.x.x (0 <= x <= 255)" << std::endl;
		exit(0);
	}

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; ip_numbers[i][j]; j++)
		{
			if(!isnumber(ip_numbers[i][j]))
			{
				std::cerr << "Ip format is wrong ! Must be : x.x.x.x (0 <= x <= 255)" << std::endl;
				exit(0);
			}
		}
		value = atoi(ip_numbers[i].c_str());
		if (value < 0 || value > 255)
		{
			std::cerr << "Ip format is wrong ! Must be : x.x.x.x (0 <= x <= 255)" << std::endl;
			exit(0);
		}
	}
}

std::vector<std::string> split(std::string s, std::string delimiter) 
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}
#include "../include/Bot.hpp"
#include "../include/Parser.hpp"

Bot::Bot(std::string user, std::string nick, std::string pass, std::string hostname, int port) : _on(true), _nick(nick), _user(user)
{
	if (!init(hostname, port))
	{
		std::cerr << "Error: initialisation failed\n";
		_on = false;
		return;
	}
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Error: fcntl failure\n";
		_on = false;
		return;
	}
	_botState = JOINING_SERVER;
	login(pass);
}

Bot::Bot()
{
}

bool Bot::init(std::string hostname, int port)
{
	struct hostent	*host = gethostbyname(hostname.c_str());
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0)
	{
		std::cerr << "socket: error" << std::endl;
		return false;
	}
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr = (*(struct in_addr*)host->h_addr);
	memset(&(_addr.sin_zero), '\0', 8);
	if (connect(_fd, (ptrsockaddr)&_addr, sizeof(sockaddr)) == -1)
	{
		close(_fd);
		return false;
	}
	_ufd->fd = _fd;
	_ufd->events = POLLIN;
	return true;
}

void Bot::login(std::string pass)
{
	sendMessage("CAP LS 302");
	sendMessage("PASS " + pass);
	sendMessage("NICK " + _nick);
	sendMessage("USER " + _nick + " 0 * :" + _user);
	sendMessage("CAP END");
}

void Bot::start(void)
{
	while (_on)
	{
		waitForInput();
	}
}

void Bot::sendMessage(std::string message)
{
	if (message[message.size() - 1] != '\n')
	{
		message += "\r\n";
	}
	if (send(_fd, message.c_str(), message.size(), 0) == -1)
	{
		std::cerr << "Error: sending message\n";
		_on = false;
	}
}

std::vector<std::string> Bot::getMessages()
{
	std::string messageBuffer = _messageBuffer;
	std::vector<std::string> output;

	char buf[512];
	int size = recv(this->_fd, buf, sizeof(buf) - 1, 0);
	if (size == -1)
		std::cout << "Error\n"; // Error

	buf[size] = '\0';

	for(int i = 0; i < size; i++)
	{
		if ((buf[i] == '\r' || buf[i] == '\n' || buf[i] == 10) && !messageBuffer.empty())
		{
			output.push_back(messageBuffer);
			messageBuffer = "";
			while(buf[i] == '\r' || buf[i] == '\n' || buf[i] == 10)
				i++;
			i--;
		}
		else
			messageBuffer += buf[i];
	}
	_messageBuffer = messageBuffer;
	return output;
}

void Bot::waitForInput()
{
	int update_count = poll(_ufd, 1, -1);

	if (update_count == -1)
	{
		std::cerr << "Poll error\n";
		shutdown();
		return;
	}

	if (_ufd->revents & POLLIN)
	{
		std::vector<std::string>	messages = getMessages();
		std::string	first_arg;

		for(int i = 0; i < static_cast<int>(messages.size()); i++)
			answerToMessage(messages[i]);
	}
	if (_ufd->revents & POLLHUP)
	{
		std::cout << "Connection to server lost\n";
		shutdown();
	}
}

void Bot::answerToMessage(std::string message)
{
	std::string first_arg = Parser::getFirstArgFromCommand(message);
	if (first_arg.empty())
		return;
	std::string	arg;
	arg = Parser::getArg(message, 1);

	if (message == ":MyBot!MyBot@ JOIN #botirc")
	{
		std::cout << "IN CHANNEL\n";
		_botState = IN_CHANNEL;
	}
	else if (arg == "001")
	{
		std::cout << "BOT LOGIN\n";
		sendMessage("JOIN #botirc");
		_botState = JOINING_CHANNEL;
	}
	else if (arg == "KICK" && Parser::getArg(message, 3) == "MyBot")
	{
		std::cout << "BOT KICK\n";
		shutdown();
	}
	else if (arg == "473" || arg == "475" || arg == "471" || arg == "403"
			|| arg == "405" || arg == "519")
	{
		std::cout << "Error: " + arg + "\n";
		shutdown();
		return ;
	}
	else if (Parser::getLastArg(message, 1) == "JOIN #botirc")
	{
		std::string name;
		for (size_t i = 1; i < first_arg.size() && first_arg[i] != '!'; i++)
			name += first_arg[i];
		sendMessage("PRIVMSG #botirc :Hello " + name + " !");
	}
}

int Bot::getFd(void)
{
	return _fd;
}

void Bot::shutdown()
{
	if (_on == false)
		return;

	if (_botState == JOINING_SERVER)
		std::cout << "Shutdown : Bot couldn't join server\n";
	else if (_botState == JOINING_CHANNEL)
	{
		std::cout << "Shutdown : Bot couldn't join channel\n";
		sendMessage("QUIT");
	}
	else
	{
		std::cout << "Shutdown : Bot is no longer in channel\n";
		sendMessage("QUIT");
	}
	close(_fd);
	_on = false;
}

Bot::~Bot()
{
}

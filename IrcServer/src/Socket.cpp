#include "../include/Socket.hpp"

Socket::Socket(int port) : _port(port), _max_usr_nbr(3)
{
	this->_fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd_socket < 0)
	{
		std::cerr << "socket: error" << std::endl;
		exit(0);
	}

	if (fcntl(_fd_socket, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "fcntl: error" << std::endl;
		this->shutdown();
		exit(0);
	}

	int opt = 1;
	if (setsockopt(this->_fd_socket,
				SOL_SOCKET,
				SO_REUSEADDR,
				&opt,
				sizeof(opt)))
	{
		std::cerr << "setsockopt: error" << std::endl;
		this->shutdown();
		exit(0);
	}

	setIp();
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = inet_addr(_ip.c_str());
	this->_address.sin_port = htons(this->_port);
	this->ircbind();
	std::cout << "IP :" << getIp() << '\n';
	this->irclisten();
}

void	Socket::ircbind()
{
	// Forcefully attaching socket to the port
	this->_addrlen = (socklen_t)sizeof(this->_address);
	if (bind(
		this->_fd_socket,
		(sockaddr*)&(this->_address),
		_addrlen) < 0)
	{
		std::cerr << "bind: error" << std::endl;
		this->shutdown();
		exit(0);
	}
}

void	Socket::irclisten(void)
{
	if (listen(this->_fd_socket, this->_max_usr_nbr) < 0)
	{
		std::cerr << "listen: error" << std::endl;
		this->shutdown();
		exit(0);
	}
	std::cout << "Configuration complete" << std::endl;
}

std::string Socket::getIp(void) const
{
	return _ip;
}

void Socket::setIp(void)
{
	char hostbuffer[256];
	char *IPbuffer;
	struct hostent *host_entry;
	int hostname;

	hostname = gethostname(hostbuffer, sizeof(hostbuffer));
	host_entry = gethostbyname(hostbuffer);
	IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
	_ip = std::string(IPbuffer);
}

int Socket::getFD() const
{
	return _fd_socket;
}

int	Socket::ircaccept(void)
{
	int	new_socket = accept(
						this->_fd_socket,
						(sockaddr*)&(this->_address),
						&(this->_addrlen)
					);
	if (new_socket < 0)
	{
		std::cerr << "accept: error" << std::endl;
		this->shutdown();
		exit(0);
	}
	return new_socket;
}

void	Socket::shutdown()
{
	close(this->_fd_socket);
}

Socket::~Socket()
{
	this->shutdown();
}

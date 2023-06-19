#ifndef IRCSOCKET_CLASS_HPP
# define IRCSOCKET_CLASS_HPP
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <netdb.h>
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

class Socket
{
private:
	int			_fd_socket;
	const int	_port;
	int			_max_usr_nbr;
	std::string	_ip;
	sockaddr_in	_address;
	socklen_t	_addrlen;

	void	ircbind(void);
	void	irclisten(void);
public:
	std::string	getIp(void) const;
	int		getFD() const;
	void	setIp(void);
	void	shutdown(void);
	int		ircaccept(void);
	Socket(int port);
	~Socket();
};
#endif

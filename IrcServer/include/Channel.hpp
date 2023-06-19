#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <iostream>
# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include "../include/Client.hpp"
#include "../include/rpl_defines.hpp"

class ChannelModule;

class Channel
{
public:
	Channel(std::string topic, Client & op, ChannelModule& channelModule);
	~Channel();

	void	addClient(Client &);
	void	addInvite(Client &);
	void	addOp(Client &);
	void	removeClient(Client &);
	void	removeOp(Client &);
	void	removeInvite(Client &);

	std::string	getName(void) const;
	void	setTopic(std::string);
	std::string	getTopic(void) const;
	std::vector<Client*> & getClients();
	std::vector<Client*> & getInvites();
	std::string			getDate();

	bool	isInChannel(Client&);
	bool	isOp(Client&);
	bool	isInvite(Client&);
	void	sendMsgToAllUsers(std::string message, Client& sender);
	void	sendMsgToOperators(std::string message, Client& sender);

	bool	getModeInvite(void) const;
	bool	getModeTopic(void) const;
	bool	getModeKey(void) const;
	bool	getModeUserLimit(void) const;
	std::string	getMode(void);

	void	setModeInvite(bool);
	void	setModeTopic(bool);
	void	setModeKey(bool);
	void	setModeUserLimit(bool);

	void	setPassword(std::string);
	void	setUserLimit(size_t	us);

	std::string	getUsersInChannel(void);
	size_t	getUserLimit(void) const;
	std::string	getPassword(void) const;

	typedef struct	s_mode
	{
		bool		inviteonly;
		bool		topic;
		bool		key;
		bool		userlimit;
		bool		op;
	}	t_mode;
private:
	Channel();

	ChannelModule&			_channelModule;
	std::string				_name;
	std::string				_topic;
	std::vector<Client*>	_clients;
	std::vector<Client*>	_invites;
	std::vector<Client*>	_ops;
	std::string	_date;

	size_t					_userlimit;
	std::string				_password;
	const int				_maxTopicLenght;

	t_mode	_mode;
};

#endif

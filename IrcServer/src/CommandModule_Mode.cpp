#include "../include/CommandModule.hpp"
#include "../include/Server.hpp"
#include "../include/Channel.hpp"

void CommandModule::handleModeCommand(Client &client, std::string const &msg)
{
	std::string channelName = getArg(msg, 1);
	Channel * channel_ptr = _channelModule.isChannel(channelName);

	if (getArgsCount(msg) < 2)
		return client.sendMessage(ERR_461_NEEDMOREPARAMS);
	if (!channel_ptr)
		return client.sendMessage(ERR_403_NOSUCHCHANNEL);
	if (!channel_ptr->isInChannel(client))
		return client.sendMessage(ERR_442_NOTONCHANNEL);

	if (getArgsCount(msg) == 2)
	{
		client.sendMessage(RPL_324_CHANNELMODE);
		client.sendMessage(RPL_329_CREATIONTIME);
		return ;
	}

	if (!channel_ptr->isOp(client))
		return client.sendMessage(ERR_482_CHANOPRIVSNEEDED);

	std::string mode_str = getArg(msg, 2);
	if (!mode_str.empty() && mode_str[0] != '+' && mode_str[0] != '-')
		mode_str = '+' + mode_str;
	std::string params = getLastArg(msg, 3);
	std::string param;
	bool	state = true;

	Channel::t_mode	mode;

	mode.inviteonly = false;
	mode.topic = false;
	mode.key = false;
	mode.userlimit = false;
	mode.op = false;

	std::string::iterator it;
	std::string	send_mode = "+";
	int	index_params = 0;
	std::string list_params = "";
	for (it = mode_str.begin(); it != mode_str.end(); it++)
	{
		if ((*it == '+' || *it == '-') && state != (*it == '+'))
		{
			if (send_mode.back() == '+' || send_mode.back() == '-')
				send_mode.pop_back();
			send_mode += *it;
			state = (*it == '+');
			mode.inviteonly = !state;
			mode.topic = !state;
			mode.key = !state;
			mode.userlimit = !state;
			mode.op = !state;
			continue;
		}
		if (*it == '+' || *it == '-')
			continue;

		if (!(*it == 'i' || *it == 't' || *it == 'k' || *it == 'o' || *it == 'l'))
		{
			client.sendMessage(ERR_472_UNKNOWNCHAR);
			continue;
		}
		if (*it == 'i')
		{
			if (state !=mode.inviteonly)
			{
				channel_ptr->setModeInvite(state);
				mode.inviteonly = state;
			}
			else
				continue;
		}
		else if (*it == 't')
		{
			if (state != mode.topic)
			{
				channel_ptr->setModeTopic(state);
				mode.topic = state;
			}
			else
				continue;
		}
		else if (*it == 'k')
		{
			param = getArg(params, index_params);
			index_params++;
			if (state && !mode.key)
			{
				if (param.empty())
				{
					client.sendMessage(ERR_696_INVALIDMODEPARAM_KEY);
					continue;
				}
				channel_ptr->setModeKey(true);
				mode.key= true;
				channel_ptr->setPassword(param);
				list_params += ' ' + param;
			}
			else if (!state && mode.key)
			{
				if (channel_ptr->getPassword() != param)
				{
					client.sendMessage(ERR_696_INVALIDMODEPARAM_KEY);
					continue;
				}
				channel_ptr->setModeKey(false);
				mode.key = false;
				list_params += ' ' + param;
			}
			else
				continue;
		}
		else if (*it == 'o')
		{
			param = getArg(params, index_params);
			index_params++;
			Client	*clt = _server.isClient(param);
			if (!clt || !channel_ptr->isInChannel(*clt))
			{
				std::string nick = (*clt).getNickname();
				client.sendMessage(ERR_401_NOSUCHNICK);
				continue;
			}
			if (state && !mode.key)
			{
				if (param.empty())
				{
					client.sendMessage(ERR_696_INVALIDMODEPARAM_OP);
					continue;
				}
				mode.op= true;
				channel_ptr->addOp(*clt);
				list_params += ' ' + param;
			}
			else if (!state && mode.key)
			{
				if (param.empty())
				{
					client.sendMessage(ERR_696_INVALIDMODEPARAM_OP);
					continue;
				}
				mode.op = false;
				channel_ptr->removeOp(*clt);
				list_params += ' ' + param;
			}
			else
				continue;
		}
		else if (*it == 'l')
		{
			if (state && !mode.userlimit)
			{
				bool valid = true;
				param = getArg(params, index_params);
				if (param.empty())
				{
					client.sendMessage(ERR_696_INVALIDMODEPARAM_UL);
					continue;
				}
				index_params++;
				if (param.size() >= 4)
				{
					client.sendMessage(ERR_696_INVALIDMODEPARAM_UL);
					continue;
				}
				std::cout << param << std::endl;
				for (size_t i = 0; i < param.size(); i++)
				{
					std::cout << param[i] << " " << isdigit(param[i]) << std::endl;
					if (!isdigit(param[i]))
					{
						client.sendMessage(ERR_696_INVALIDMODEPARAM_UL);
						valid = false;
						break;
					}
				}
				if (!valid)
					break;
				mode.userlimit= true;
				channel_ptr->setModeUserLimit(true);
				channel_ptr->setUserLimit(std::atoi(param.c_str()));
				list_params += ' ' + param;
			}
			else if (!state && mode.userlimit)
			{
				channel_ptr->setModeUserLimit(false);
				mode.userlimit = false;
			}
			else
				continue;
		}
		send_mode += *it;
	}
	if (list_params.size() > 0)
		send_mode += list_params;
	client.sendMessage(":" + client.getID() + " MODE " + channel_ptr->getName() + " " + send_mode + "\r\n");
	channel_ptr->sendMsgToAllUsers(":" + client.getID() + " MODE " + channel_ptr->getName() + " " + send_mode + "\r\n", client);
}

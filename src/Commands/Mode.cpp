#include "Commands.hpp"
#include <algorithm>

Mode::Mode(Server *server) : Command("MODE", server) { }

Mode::~Mode(void) { }

void Mode::invoke(Client *client, Message *message)
{
    if (client->is_authenticated() && client->is_registered())
    {
        // Check if message has enough parameters
        if (message->get_params().size() < 1)
        {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for MODE command");
            return ;
        }

        std::string channel_name = message->get_params()[0];

        // Check if channel exists
        Channel *channel = _server->get_channel(channel_name);
        if (channel == NULL)
        {
            client->reply(ERR_NOSUCHCHANNEL, ":No channel " + channel_name);
            return ;
        }

        if (!channel->has_client(client))
        {
            client->reply(ERR_NOTONCHANNEL, ":You're not on channel " + channel_name);
            return ;
        }

        // Check if command was invoked only for listing the channel's modes
        if (message->get_params().size() == 1)
        {
            client->reply(RPL_CHANNELMODEIS, channel->get_name() + " " + channel->get_modes());
            return ;
        }

        if (!client->is_oper() && !channel->is_chanop(client->get_nickname()))
        {
            client->reply(ERR_NOPRIVILEGES, channel->get_name() + " " + ":Permission Denied");
            return ;
        }

        std::string arg = message->get_params()[1];
        if (arg.length() < 2) {
            client->reply(ERR_UNKNOWNMODE, channel->get_name() + " " + ":Unknown mode char");
            return ;
        }

        // Separate flag into two chars
        char sign = arg[0];
        arg = arg.substr(1, arg.length());
        if (!arg.find_first_not_of("tiokl"))
            client->reply(ERR_UNKNOWNMODE, channel->get_name() + " " + ":Unknown mode char");

        int c = 0;

        for (int i = 0; i < int(arg.length()) && sign == '+'; i++)
            if (arg[i] == 'l' || arg[i] == 'o' || arg[i] == 'k')
                c++;
        if (2 + c > int(message->get_params().size()))
        {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for MODE command");
            return ;
        }
        if (sign == '-')
            channel->unset_mode(arg, message->get_params(), client, channel->get_name());
        else if (sign == '+')
            channel->set_mode(arg, message->get_params(), client, channel->get_name());
        else
            client->reply(ERR_UNKNOWNMODE, channel->get_name() + " " + ":Unknown mode char");
    }
    else
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
}

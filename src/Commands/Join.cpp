#include "Commands.hpp"

Join::Join(Server *server) : Command("JOIN", server) { }

Join::~Join(void) { }

void Join::invoke(Client *client, Message *message)
{
    if (client->is_authenticated() && client->is_registered())
    {
        if (message->get_params().size() < 1) {
            client->reply(ERR_NEEDMOREPARAMS, ":Missing parameters! Usage: /join #<channel_name>");
            return;
        }
        std::string channel_name = message->get_params()[0];
        if (channel_name[0] != '#')
        {
            client->reply(ERR_NOSUCHCHANNEL, ":Incorrect channel name format");
            return;
        }

        Channel *channel = _server->get_channel(channel_name);
        if (channel == NULL) // New channel
        {
            if (!client->is_oper())
            {
                client->reply(ERR_NOPRIVILEGES, ":No such channel. You do not have permissions to create a new channel.");
                return ;
            }
            client->reply(ERR_NOSUCHCHANNEL, ":No such channel. Creating channel " + channel_name);
            channel = new Channel(channel_name, _server->get_hostname());
            _server->add_channel(channel);
            channel->increase_user_quantity();
            channel->join(client);
            channel->add_chanop(client);
        }
        else // Existing channel
        {
            if (channel->has_client(client))
            {
                client->reply(ERR_ALREADYONCHANNEL, channel->get_name() + " " + ":You're already on that channel");
                return;
            }
            if (channel->get_invite_only() && channel->get_invited_names().find(client->get_nickname()) == std::string::npos)
            {
                client->reply(ERR_INVITEONLYCHAN, channel->get_name() + " " + ":Cannot join channel (+i)");
                return;
            }
            if(channel->has_key() && message->get_params()[1] != channel->get_key())
            {
                client->reply(ERR_BADCHANNELKEY, channel->get_name() + " " + ":Cannot join channel (+k)");
                return;
            }
            if(channel->has_user_limit() && channel->get_user_quantity() >= channel->get_user_limit())
            {
                client->reply(ERR_CHANNELISFULL, channel->get_name() + " " + ":Cannot join channel (+l)");
                return;
            }
            channel->increase_user_quantity();
            channel->join(client);
        }

        // Channel Topic and Names
        std::vector<std::string> params;
        params.push_back(channel->get_name());
        channel->topic(client, params);
        channel->names(client);
    }
    else
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
}

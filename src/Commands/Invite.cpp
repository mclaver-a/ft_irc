#include "Commands.hpp"

Invite::Invite(Server *server) : Command("INVITE", server) {
    return ;
}

Invite::~Invite(void) {
    return ;
}

void Invite::invoke(Client *client, Message *message)
{
    if (client->is_authenticated() && client->is_registered())
    {
        if (message->get_params().size() < 2)
        {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for INVITE command!");
            return ;
        }

        Channel *channel = _server->get_channel(message->get_params()[1]);
        if (!channel)
        {
            client->reply(ERR_NOSUCHCHANNEL, ":That channel doesn't exist!");
            return ;
        }

        if (channel->get_client_names().find(client->get_nickname()) == (size_t)-1)
        {
            client->reply(ERR_NOTONCHANNEL, ":You aren't on the channel!");
            return ;
        }

        if (channel->get_chanop_names().find(client->get_nickname()) == (size_t)-1)
        {
            client->reply(ERR_NOTOPER, ":You aren't a channel operator!");
            return ;
        }

        Client *target = _server->get_client(message->get_params()[0]);

        if (target == NULL)
        {
            client->reply(ERR_NOSUCHNICK, ":No such nick in channel!");
            return ;
        }

        channel->invite(target);
        client->reply(RPL_INVITING, target->get_nickname() + " " + channel->get_name());
        target->broadcast(client, "INVITE", target->get_nickname(), channel->get_name());
    }
    else {
        client->reply(ERR_NOTREGISTERED, ":You are not registered!");
    }
}

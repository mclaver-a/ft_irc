#include "Commands.hpp"

Kick::Kick(Server *server) : Command("KICK", server) { }

Kick::~Kick(void) { }

void Kick::invoke(Client *client, Message *message)
{
    if (client->is_authenticated() && client->is_registered())
    {
        if (message->get_params().size() < 3)
        {
            client->reply(ERR_NEEDMOREPARAMS, ":Missing parameters! Usage: /kick <nickname> #<channel name>");
            return ;
        }

        Channel *channel = _server->get_channel(message->get_params()[2]);
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

        Client *target = _server->get_client(message->get_params()[1]);

        if (target == NULL)
        {
            client->reply(ERR_NOSUCHNICK, ":No such nick in channel!");
            return ;
        }

        channel->kick(client, target, "Kicked by OP");
    }
    else
        client->reply(ERR_NOTREGISTERED, ":You are not registered!");
}

#include "Commands.hpp"

Who::Who(Server *server) : Command("WHO", server) { }

Who::~Who(void) { }

void    Who::invoke(Client *client, Message *message)
{
    if (client->is_authenticated() && client->is_registered())
    {
        if (message->get_params().size() < 1)
        {
            client->reply(ERR_NEEDMOREPARAMS, ":Missing parameters! Usage: /who <channel_name>");
            return;
        }

        std::string channel_name = message->get_params()[0];
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

        std::string flag = message->get_params().size() > 1 ? message->get_params()[1] : "";

        if (flag != "" && flag != "o")
        {
            client->reply(ERR_UNKNOWNMODE, ":Flag '" + flag + "' is unknown");
            return ;
        }

        std::vector<Client *> clients;
        // List all clients on a channel
        if (message->get_params().size() == 1)
            clients = channel->get_clients();
        // List
        else
            clients = channel->get_chanop_clients();

        std::vector<Client *>::iterator it;
        for (it = clients.begin(); it != clients.end(); it++)
        {
            std::string info = channel->get_client_info(*it, channel);
            client->reply(RPL_WHOREPLY, channel->get_name() + " " + info);
        }
        client->reply(RPL_ENDOFWHO, channel->get_name() + " :End of /WHO list");
    }
    else
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    return ;
}

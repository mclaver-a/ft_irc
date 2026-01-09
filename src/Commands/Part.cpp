#include "Commands.hpp"

Part::Part(Server *server) : Command("PART", server) {
    return ;
}

Part::~Part(void) {
    return ;
}

void Part::invoke(Client *client, Message *message)
{
    if (client->is_authenticated() && client->is_registered())
    {
        if (message->get_params().size() < 1)
        {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for PART command! Usage: /part #<channel name> <parting message>");
            return ;
        }

        Channel *channel = _server->get_channel(message->get_params()[0]);
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

        std::string partingmessage = message->get_params().size() >= 1 ? message->get_params()[1] : "Ciao!";
        channel->part(client, partingmessage);
    }
    else
        client->reply(ERR_NOTREGISTERED, ":You are not registered!");
}

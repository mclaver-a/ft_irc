#include "Commands.hpp"

Notice::Notice(Server *server) : Command("NOTICE", server) {
    return ;
}

Notice::~Notice(void) {
    return ;
}

void    Notice::invoke(Client *client, Message *message)
{
    if (client->is_authenticated() && client->is_registered())
    {
        if (message->get_params().size() < 2)
        {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for NOTICE command");
            return;
        }

        std::string target_name = message->get_params()[0];
        std::string msg = ":" + message->get_params()[1];
        // Try to get target by name
        Channel *channel = _server->get_channel(target_name);
        Client  *target = _server->get_client(target_name);

        if (!channel && !target)
            return ; //silent error

        if (channel) //target is a channel
        {
            if (!channel->has_client(client))
                return ; //silent error
            channel->broadcast(client, "NOTICE", "", msg);
        }
        else //target is a user
            target->broadcast(client, "NOTICE", target_name, msg);

    }
    else
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
}

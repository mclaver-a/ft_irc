#include "Commands.hpp"

Pass::Pass(Server *server) : Command("PASS", server) { }

Pass::~Pass(void) { }

void Pass::invoke(Client *client, Message *message)
{
    if (client->is_authenticated() == true)
    {
        client->reply(ERR_ALREADYREGISTRED, "You are already registered to the network!");
        return ;
    }

    if (message->get_params().size() < 1)
    {
        client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters on PASS command");
        return ;
    }

    client->authenticate(message->get_params()[0]);
    if (!client->is_authenticated())
        client->disconnect("Disconnected due to missmatched password!");
}

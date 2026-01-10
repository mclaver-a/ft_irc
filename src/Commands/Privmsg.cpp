#include "Commands.hpp"

Privmsg::Privmsg(Server *server) : Command("PRIVMSG", server) {
    return ;
}

Privmsg::~Privmsg(void) {
    return ;
}

void    Privmsg::invoke(Client *client, Message *message)
{
    std::string target_name;
    std::string message_text;

    if (client->is_authenticated() && client->is_registered()) {

        if (message->get_params().size() < 2) {
            client->reply(ERR_NEEDMOREPARAMS, ":Missing parameters for PRIVMSG. Usage: /privmsg <target_name> <message_text> ");
            return ;
        }
        target_name = message->get_params()[0];
        message_text = message->get_params()[1];

        // Try to get target by name
        Channel *channel = _server->get_channel(target_name);
        Client  *target = _server->get_client(target_name);

        if (!channel && !target) {
            client->reply(ERR_NOSUCHNICK, ":No such nick/channel");
            return ;
        }

        if (channel) //target is a channel
        {
            if (!channel->has_client(client)) {
                client->reply(ERR_NOTONCHANNEL, ":You're not on channel " + target_name);
                return ;
            }
            channel->broadcast(client, _name, "", message_text);
        }
        else //target is a user
            target->broadcast(target, _name, target_name, message_text);

    }
    else
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
}

#include "Commands.hpp"

Topic::Topic(Server *server) : Command("TOPIC", server) {
    return ;
}

Topic::~Topic(void) {
    return ;
}

void    Topic::invoke(Client *client, Message *message)
{
    if (client->is_authenticated() && client->is_registered())
    {
        if (message->get_params().size() < 2)
        {
            client->reply(ERR_NEEDMOREPARAMS, "Missing parameters! Usage: /topic #<channel name> <new topic>");
            return ;
        }

        std::string channelname = message->get_params()[0];
        Channel *channel = _server->get_channel(channelname);

        if (!channel)
        {
            client->reply(ERR_NOSUCHCHANNEL, "No such channel!");
            return ;
        }

        if (!channel->has_client(client))
        {
            client->reply(ERR_NOTONCHANNEL, "You aren't on the channel!");
            return ;
        }

        if (channel->get_topic_restriction() && !channel->is_chanop(client->get_nickname()))
        {
            client->reply(ERR_CHANOPRIVSNEEDED, "Topic restriction is active! You must be chanop to change topic.");
            return ;
        }

        channel->topic(client, message->get_params());
    }
    else
        client->reply(ERR_NOTREGISTERED, "You aren't registered!");
}

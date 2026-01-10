#include "Commands.hpp"

Cap::Cap(Server *server) : Command("CAP", server) { }

Cap::~Cap(void) { }

void Cap::invoke(Client *client, Message *message)
{
    (void)message;
    client->reply(RPL_CAP, ":Nothing too special about this server :3");
}

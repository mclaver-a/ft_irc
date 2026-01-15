#include "Commands.hpp"

Cap::Cap(Server *server) : Command("CAP", server) { }

Cap::~Cap(void) { }

void Cap::invoke(Client *client, Message *message)
{
    (void)message;
    std::string cap_response = "Nothing special about this server :) Fuggeraboutit!\n";
    send(client->get_socket(), cap_response.c_str(), cap_response.length(), 0);
}

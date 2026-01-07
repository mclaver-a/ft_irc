#include "Commands.hpp"

Quit::Quit(Server *server) : Command("QUIT", server) {
    return ;
}

Quit::~Quit(void) {
    return ;
}

void    Quit::invoke(Client *client, Message *message) {

    std::string quit_message = "Leaving";
    if (message->get_params().size() > 0) {
        quit_message = message->get_params()[0];
    }

    std::vector<Channel *> channels = _server->list_channels();
    // Iterate over all channels in the server and leave if you are in any
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++) {
        Channel *channel = *it;
        if (channel->has_client(client)) {
            channel->quit(client, quit_message);
        }
    }
    // Send goodbye message to client
    client->disconnect(":Thank you for using the IRC Network, " + client->get_nickname() + ". Goodbye!");
    _server->on_client_disconnect(client->get_socket());
}

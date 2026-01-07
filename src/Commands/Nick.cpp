#include "Commands.hpp"

Nick::Nick(Server *server) : Command("NICK", server) {
    return ;
}

Nick::~Nick(void) {
    return ;
}

void    Nick::invoke(Client *client, Message *message) {

    // Check if message has enough parameters
    if (message->get_params().size() < 1) {
        client->reply("ERR_ERRONEUSNICKNAME", "<empty_nickname>");
        return ;
    }

    std::string nickname = message->get_params()[0];

    // Check if nickname is empty
    if (nickname.empty()) {
        client->reply("ERR_NONICKNAMEGIVEN", "<empty_nickname>");
        return ;
    }

    // Check if nickname is already in use
    if (_server->get_client(nickname)) {
        client->reply("ERR_NICKNAMEINUSE", "Your chosen nickname is already in use!");
        return;
    }

    // Check if nickname is valid
    for (size_t i = 0; i < nickname.length(); i++) {
        if(!std::isalnum(nickname[i]) && nickname[i] != '_') {
            client->reply("ERR_ERRONEUSNICKNAME", ":Nickname can only contain letters, numbers and underscores");
            return ;
        }
    }

    // Set new nickname and attempt to register client
    client->set_nickname(nickname);
    client->register_client();
    return ;
}

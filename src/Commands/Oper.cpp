#include "Commands.hpp"

Oper::Oper(Server *server) : Command("OPER", server) { }

Oper::~Oper(void) { }

void Oper::invoke(Client *client, Message *message)
{
    if (client->is_authenticated() && client->is_registered())
    {
        if (message->get_params().size() < 1)
        {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for OPER command");
            return ;
        }

        std::string target_name = message->get_params()[0];
        Client *target = _server->get_client(target_name);

        if (target)
        {
            if (!target->is_registered())
            {
                client->reply(ERR_NOTREGISTERED, ":" + target_name + " has not registered");
                return ;
            }
            if (target->is_oper())
            {
                client->reply(RPL_YOUREOPER, ":User is already an IRC operator");
                return ;
            }

            if (message->get_params().size() == 1) // Oper someone else
            {
                if (!client->is_oper()) {
                    client->reply(ERR_NOPRIVILEGES, ":Permission Denied");
                    return ;
                } else
                    target->oper(_server->get_oper_password());
            }
            else // Oper yourself
                target->oper(message->get_params()[1]);

            if (target->is_oper())
            {
                if (client->get_nickname() != target_name)
                    client->reply(RPL_YOUREOPER, ":" + target_name + " was assigned as a new IRC operator");
                target->reply(RPL_YOUREOPER, ":You are now an IRC operator");
            }
            else
                client->reply(ERR_PASSWDMISMATCH, ":Operator password is incorrect");
        }
        else
            client->reply(ERR_NOSUCHNICK, ":No such nick/channel");
    }
    else
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclaver- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 19:04:32 by mclaver-          #+#    #+#             */
/*   Updated: 2025/11/25 19:04:34 by mclaver-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include <ostream>

User::User(Server *server) : Command("USER", server) {
    return ;
}

User::~User(void) {
    return ;
}

void User::invoke(Client *client, Message *message)
{
    std::string username;
    std::string realname;
    (void)message;

    if (client->is_registered() == true)
    {
        client->reply(ERR_ALREADYREGISTRED, "You are already registered to the network!");
        return ;
    }

    if (message->get_params().size() < 4)
    {
        client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters on USER command");
        return ;
    }

    // Check if username is composed of valid characters (alpha-numeric, underscore, dash and dot only)
    username = message->get_params()[0];
    for (size_t i = 0; i < username.length(); i++) {
        if(!std::isalnum(username[i]) && username[i] != '_' && username[i] != '-' && username[i] != '.') {
            client->reply(ERR_ERRONEUSUSERNAME, ":Invalid username. Use only alpha-numeric characters, underscore, dash and dot.");
            return ;
        }
    }

    // Check if realname is composed of valid characters (any printable character - including spaces - is allowed)
    realname = message->get_params()[3];
    for (size_t i = 0; i < realname.length(); i++) {
        if(!std::isprint(realname[i])) {
            client->reply(ERR_ERRONEUSREALNAME, ":Invalid realname. Use only printable characters.");
            return ;
        }
    }

    client->set_username(username);
    client->set_realname(realname);
    client->register_client();
}

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

    //TODO: check legality of username and realname. also check if the user is already registered.

    client->set_username(username);
    client->set_realname(realname);
    client->register_client();

    return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclaver- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 19:02:28 by mclaver-          #+#    #+#             */
/*   Updated: 2025/11/25 19:02:36 by mclaver-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

Cap::Cap(Server *server) : Command("CAP", server) {
    return ;
}

Cap::~Cap(void) {
    return ;
}

void Cap::invoke(Client *client, Message *message) {
    (void)message;
    //TODO figure out what the fuck these codes mean
    client->reply("302", ":Nothing too special about this server :3");
}

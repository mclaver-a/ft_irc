/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclaver- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 19:04:05 by mclaver-          #+#    #+#             */
/*   Updated: 2025/11/25 19:04:07 by mclaver-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

Pass::Pass(Server *server) : Command("PASS", server) {
    return ;
}

Pass::~Pass(void) {
    return ;
}

void Pass::invoke(Client *client, Message *message)
{
    //TODO check if the client is already authenticated
    //

    client->authenticate(message->get_params()[0]);
    if (!client->is_authenticated())
        client->disconnect("Disconnected due to missmatched password!");

}

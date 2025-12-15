/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclaver- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 20:17:11 by mclaver-          #+#    #+#             */
/*   Updated: 2025/12/15 20:17:21 by mclaver-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Client/Client.hpp"
#include "../Message/Message.hpp"
#include <string>

class Channel {
    private:
        std::string             _name;
        std::string             _hostname;
        std::string             _topic;
        std::string             _key;
        int                     _user_limit;
        int                     _user_quantity;
        std::vector<Client *>   _clients;
        std::vector<Client *>   _op_clients;
        std::vector<Client *>   _invited_clients;
        bool                    _topic_restriction;
        bool                    _invite_only;
        bool                    _has_key;
        bool                    _has_user_limit;

    public:
        Channel(std::string name, std::string hostname);
        Channel(const Channel &other);
        ~Channel(void);
        Channel &operator=(const Channel &other);
        //add join, quit, topic, etc...
};

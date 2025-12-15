/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclaver- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 20:17:01 by mclaver-          #+#    #+#             */
/*   Updated: 2025/12/15 20:17:02 by mclaver-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, std::string hostname)
    : _name(name),
    _hostname(hostname),
    _topic(""),
    _key(""),
    _user_limit(0),
    _user_quantity(0),
    _topic_restriction(true),
    _invite_only(false),
    _has_key(false),
    _has_user_limit(false) {
    return ;
}

Channel::Channel(const Channel &other) {
    *this = other;
}

Channel::~Channel(void) {
    // Clear the clients vector
    this->_clients.clear();
    return ;
}

Channel &Channel::operator=(const Channel &other) {
    this->_name = other._name;
    this->_hostname = other._hostname;
    this->_topic = other._topic;
    this->_key = other._key;
    this->_user_limit = other._user_limit;
    this->_user_quantity = other._user_quantity;
    this->_clients = other._clients;
    this->_op_clients = other._op_clients;
    this->_invited_clients = other._invited_clients;
    this->_topic_restriction = other._topic_restriction;
    this->_invite_only = other._invite_only;
    this->_has_key = other._has_key;
    this->_has_user_limit = other._has_user_limit;
    return *this;
}

//add join, quit, topic, etc...

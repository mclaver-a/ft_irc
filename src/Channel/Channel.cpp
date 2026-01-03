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
void    Channel::join(Client *client) {
    this->_clients.push_back(client);
    this->broadcast(client, "JOIN", this->get_name(), "");
    return ;
}

void    Channel::quit(Client *client, std::string message) {
    this->remove_chanop(client);
    this->broadcast(client, "QUIT", this->get_name(), message);
    this->leave(client);
    return ;
}

void    Channel::leave(Client *client) {
    // Remove client from channel operators
    std::vector<Client *>::iterator it_op = std::find(this->_op_clients.begin(), this->_op_clients.end(), client);
    if (it_op != this->_op_clients.end())
        this->_op_clients.erase(it_op);

    // Remove client from channel
    std::vector<Client *>::iterator it = std::find(this->_clients.begin(), this->_clients.end(), client);
    if (it != this->_clients.end()) {
        this->_clients.erase(it);
        this->_user_quantity--;
    }
    return ;
}

void    Channel::broadcast(Client *sender, std::string command, std::string target, std::string message) {
    if (target.empty())
        target = this->get_name();
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if (command == "PRIVMSG" && *it == sender)
            continue ;
        (*it)->broadcast(sender, command, target, message);
    }
    return ;
}

//operator management

void    Channel::add_chanop(Client *client) {
    this->_op_clients.push_back(client);
    return ;
}

void    Channel::remove_chanop(Client *client) {
    std::vector<Client *>::iterator it = std::find(this->_op_clients.begin(), this->_op_clients.end(), client);
    if (it != this->_op_clients.end())
        this->_op_clients.erase(it);
    return ;
}

bool    Channel::is_chanop(std::string nickname) {
    for (std::vector<Client *>::iterator it = this->_op_clients.begin(); it != this->_op_clients.end(); it++)
        if ((*it)->get_nickname() == nickname)
            return true;
    return false;
}

//useless shit

std::string             Channel::get_name(void) const {
    return this->_name + ":" + this->_hostname;
}

std::string             Channel::get_topic(void) const {
    return this->_topic.empty() ? this->_topic : ":" + this->_topic;
}

std::string             Channel::get_key(void) const {
    return this->_key;
}

std::string             Channel::get_modes(void) {
    std::string modes = "+";
    std::string params = "";
    if (this->_topic_restriction)
        modes += "t";
    if (this->_invite_only)
        modes += "i";
    if (this->_has_key) {
        modes += "k";
        params += this->_key;
    }
    if (this->_op_clients.size() > 0) {
        modes += "o";
        if (!params.empty())
            params += " ";
        params += this->get_chanop_names();
    }
    if (this->has_user_limit()) {
        modes += "l";
        if (!params.empty())
            params += " ";
        std::stringstream ss;
        ss << this->_user_limit;
        params += ss.str();
    }

    // Concatenate modes and params
    std::string response;
    if (params.empty())
        response = modes;
    else
        response = modes + " " + params;

    return response;
}

int                     Channel::get_user_limit(void) const {
    return this->_user_limit;
}


int                     Channel::get_user_quantity(void) const {
    return this->_user_quantity;
}

Client *Channel::get_client(std::string nickname, std::vector<Client *> &clients) {
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); it++)
        if ((*it)->get_nickname() == nickname)
            return *it;
    return NULL;
}

std::vector<Client *>   Channel::get_clients(void) {
    return this->_clients;
}

std::vector<Client *>   Channel::get_chanop_clients(void) {
    return this->_op_clients;
}

std::string             Channel::get_client_names(void) {
    std::string names;
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
        names += is_chanop((*it)->get_nickname()) ? "@" : "";
        names += (*it)->get_nickname() + " ";
    }
    return names;
}

std::string             Channel::get_chanop_names(void) {
    std::string names;
    for (std::vector<Client *>::const_iterator it = this->_op_clients.begin(); it != this->_op_clients.end(); it++)
        names += (*it)->get_nickname() + " ";
    return names;
}

std::string             Channel::get_invited_names(void) {
    std::string names;
    for (std::vector<Client *>::const_iterator it = this->_invited_clients.begin(); it != this->_invited_clients.end(); it++) {
        names += (*it)->get_nickname() + " ";
    }
    return names;
}

std::string             Channel::get_client_info(Client *client, Channel *channel) {
    std::string info = "";
    info += client->get_nickname();
    info += " " + client->get_username();
    info += " " + client->get_hostname();
    info += " " + client->get_hostname();
    info += " " + client->get_nickname();
    info += " H";
    if (channel->is_chanop(client->get_nickname()))
        info += "@";
    info += " :" + client->get_realname();
    return info;
}

bool                    Channel::get_topic_restriction(void) const {
    return this->_topic_restriction;
}

bool                    Channel::get_invite_only(void) const {
    return this->_invite_only;
}

bool    Channel::has_key(void) const {
    return this->_has_key;
}

bool    Channel::has_user_limit(void) const {
    return this->_has_user_limit;
}

bool    Channel::has_client(Client *client) {
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
        if ((*it)->get_nickname() == client->get_nickname())
            return true;
    return false;
}

void                Channel::set_topic(const std::string topic) {
    this->_topic = topic;
    return ;
}

void               Channel::increase_user_quantity(void) {
    this->_user_quantity++;
    return ;
}

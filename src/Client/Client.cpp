/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclaver- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2005/11/18 18:40:30 by mclaver-          #+#    #+#             */
/*   Updated: 2005/11/18 18:40:45 by mclaver-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(std::string server_hostname, int fd, int port, std::string password, const std::string &hostname)
    : _server_hostname(server_hostname),
    _socket(fd),
    _port(port),
    _password(password),
    _disconnected(false),
    _authenticated(false),
    _registered(false),
    _oper(false),
    _nickname(""),
    _username(""),
    _realname(""),
    _hostname(hostname),
    _message_buffer("") {

    std::cout << "Client on " << _hostname << ":" << _port << " created" << std::endl;
    return ;
}

Client::Client(const Client &other) {
    *this = other;
    return ;
}

Client::~Client(void) {
    if (_socket != -1) {
        close(_socket);
    }
    std::cout << "Client destroyed" << std::endl;
    return ;
}

Client &Client::operator=(const Client &other) {
    this->_server_hostname = other._server_hostname;
    this->_socket = other._socket;
    this->_port = other._port;
    this->_password = other._password;
    this->_disconnected = other._disconnected;
    this->_authenticated = other._authenticated;
    this->_registered = other._registered;
    this->_oper = other._oper;
    this->_nickname = other._nickname;
    this->_username = other._username;
    this->_realname = other._realname;
    this->_hostname = other._hostname;
    return *this;
}

void        Client::reply(std::string code, std::string message)
{
    std::string hostname_str;
    std::string code_str;
    std::string nickname_str;

    hostname_str = ":" + _server_hostname + " ";
    code_str = code.empty() ? "" : code + " ";
    nickname_str = _nickname.empty() ? "unregistered " : _nickname + " ";

    // Format ":<server_hostname> <code> <nickname> :<message>\r\n"
    std::string reply;
    reply = hostname_str + code_str + nickname_str + message + "\n";

    std::cout << "Reply: " << reply << std::endl;

    // Send reply to client
    send(_socket, reply.c_str(), reply.length(), 0);
}

std::string Client::get_server_hostname(void) const {
    return _server_hostname;
}

int         Client::get_socket(void) const {
    return _socket;
}

int         Client::get_port(void) const {
    return _port;
}

std::string Client::get_nickname(void) const {
    return _nickname;
}

std::string Client::get_username(void) const {
    return _username;
}

std::string Client::get_realname(void) const {
    return _realname;
}

std::string Client::get_hostname(void) const {
    return _hostname;
}

std::string Client::get_buffer(void) const {
    return _message_buffer;
}

bool        Client::is_disconnected(void) const {
    return _disconnected;
}

bool        Client::is_authenticated(void) const {
    return _authenticated;
}

bool        Client::is_registered(void) const {
    return _registered;
}

bool        Client::is_oper(void) const {
    return _oper;
}

bool        Client::has_nickname(void) const {
    return !_nickname.empty();
}

void        Client::set_username(const std::string &username) {
    _username = username;
}

void        Client::set_realname(const std::string &realname) {
    _realname = realname;
}

void        Client::set_nickname(const std::string &nickname)
{
    _nickname = nickname;
}

void        Client::set_buffer(std::string message) {
    this->_message_buffer += message;
}

void        Client::clear_buffer(void) {
    this->_message_buffer.clear();
}

void        Client::register_client(void)
{
    if (!this->has_nickname())
    {
        //error message no nickname: 431
        reply("431", ":You must choose a nickname before registering");
    } else if (!this->get_username().empty() && !this->get_realname().empty()) {

        _registered = true;
        //welcome code: 001
        reply("001", ":Welcome to the Internet Relay Network " + this->get_nickname() + "!" + this->get_username() + "@" + this->get_hostname());
    }
    std::cout << "im a fucking retard: " << this->get_username() << " " << this->get_realname() << std::endl;
}

void        Client::authenticate(std::string password) {
    if (password == _password)
        _authenticated = true;
    return ;
}

void        Client::disconnect(std::string message) {
    if (!_disconnected) {
        //creo que 101 significa disconnect
        reply("101", message);
        close(_socket);
        _disconnected = true;
    }
}

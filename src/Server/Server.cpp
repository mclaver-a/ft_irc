/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclaver- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 18:41:02 by mclaver-          #+#    #+#             */
/*   Updated: 2025/11/18 18:41:28 by mclaver-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

Server::Server(std::string port, std::string password)
    : _running(false),
    _socket(-1),
    _port(port),
    _password(password),
    _hostname("127.0.0.1"),
    _oper_password("chungus"),//cambiar + adelante por lo que definamos en Client
    _info("IRC server by dmarijan and mclaver-...fuggetaboutit")
{
    return ;
}

Server::Server(const Server &other) {
    *this = other;
    return ;
}

Server::~Server(void) {
    // Close the server socket
    if (_socket != -1) {
        close(_socket);
    }
    // Close all client sockets
    for (size_t i = 2; i < _pollfds.size(); i++) {
        if (_pollfds[i].fd != -1)
            close(_pollfds[i].fd);
    }

    // TODO Delete all clients from the map

    // TODO Delete all channels from the vector

    // TODO Delete commands

    return ;
}

Server                  &Server::operator=(const Server &other) {
    this->_running = other._running;
    this->_socket = other._socket;
    this->_port = other._port;
    this->_password = other._password;
    this->_hostname = other._hostname;
    this->_oper_password = other._oper_password;
    this->_info = other._info;
    this->_pollfds = other._pollfds;
    return *this;
}

void Server::on_client_connect(void)
{
    sockaddr_in  client_address;
    socklen_t    client_address_size = sizeof(client_address);
    while (true)
    {
        std::cout << "hi" << std::endl;
        int client_fd = accept(_socket, (struct sockaddr *)&client_address, &client_address_size);
        if (client_fd)
        {
            std::cout << "127.0.0.1" << ":" << _port << " has connected" << std::endl;
            char buffer[1024] = {0};
            recv(client_fd, buffer, sizeof(buffer), 0);
            std::cout << "Message from Client: " << buffer << std::endl;
            close(client_fd);
        }
    }
}

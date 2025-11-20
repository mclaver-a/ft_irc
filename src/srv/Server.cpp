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
# include "../util/utils.hpp"

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

//basicamente, esto tiene un loop infinito donde va vigilando, mediante la funcion poll, si en cada fd
// del vector PollFds, ha sucedido el evento que le hemos asignado. le asignamos POLLIN: there is data to read.
// por lo tanto, si hay POLLIN en el socket del servidor, hay un usuario nuevo. si hay POLLIN en stdin,
// tenemos que leer un comando
// ESTO TB DEBERIA ESTAR EN Server.cpp
void event_loop(int socket)
{
    bool running = true;

    std::vector<pollfd> pollfds;
    addPollfd(pollfds, socket, POLLIN);
    addPollfd(pollfds, STDIN_FILENO, POLLIN);

    while (running)
    {
        int npolls = poll(pollfds.data(), pollfds.size(), 0);

        if (npolls == -1)
            throw std::runtime_error("Cannot poll for events: " + std::string(strerror(errno)));

        // If no events happened, continue
        if (npolls == 0)
            continue;

        if (pollfds[0].revents & POLLIN) {
            try {
                on_client_connect();
            } catch (std::exception &e) {
                throw std::runtime_error("Cannot accept client connection: " + std::string(strerror(errno)));
            }
        }

        //TODO: mas mierda por aqui...

    }
}

//will open the general socket and bind it to arg port.
//TODO: is password arg necessary?
//TODO: is setting reuse necessary?
//TODO: remove the return logic. when the struct is working we wont need to return anything
// ESTO TB deberia estar en Server.cpp
int open_general_socket(std::string port, std::string password)
{
    int socket = socket(AF_INET, SOCK_STREAM, 0);
    if (socket == -1)
        throw std::runtime_error("Error: creating server socket");

    int enable = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
        throw std::runtime_error("Error: changing server options");

    if (fcntl(socket, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Error: changing server socket to nonblock");

    struct sockaddr_in srv_addr = {0};
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(std::atoi(port.c_str()));
    srv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1)
        throw std::runtime_error("Error: binding server socket to port");

    if (listen(socket, 85) == -1)
        throw std::runtime_error("Error: reading from server socket");
    return socket;
}

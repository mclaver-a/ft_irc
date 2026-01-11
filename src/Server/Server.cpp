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
# include "../Commands/Commands.hpp"
# include "../Utils/Utils.hpp"
#include <exception>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
# include "../Client/Client.hpp"

Server::Server(std::string port, std::string password)
    : _running(false),
    _socket(-1),
    _port(port),
    _password(password),
    _hostname("127.0.0.1"),
    _oper_password(g_oper_password),
    _info("IRC server by dmarijan and mclaver-...fuggetaboutit")
{
    _commands["USER"] = new User(this);
    _commands["PASS"] = new Pass(this);
    _commands["CAP"] = new Cap(this);
    _commands["NICK"] = new Nick(this);
    _commands["JOIN"] = new Join(this);
    _commands["OPER"] = new Oper(this);
    _commands["UNOPER"] = new UnOper(this);
    _commands["WHO"] = new Who(this);
    _commands["MODE"] = new Mode(this);
    _commands["LIST"] = new List(this);
    _commands["QUIT"] = new Quit(this);
    _commands["INVITE"] = new Invite(this);
    _commands["KICK"] = new Kick(this);
    _commands["TOPIC"] = new Topic(this);
    _commands["PART"] = new Part(this);
    _commands["PRIVMSG"] = new Privmsg(this);
}

Server::Server(const Server &other)
{
    *this = other;
}

Server::~Server(void)
{
    // Close the server socket
    if (_socket != -1)
        close(_socket);

    // Close all client sockets
    for (size_t i = 2; i < _pollfds.size(); i++)
        if (_pollfds[i].fd != -1)
            close(_pollfds[i].fd);

    // Delete all clients from the map
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
            delete it->second;

    // Delete all channels from the vector
    for (size_t i = 0; i < _channels.size(); i++)
            delete _channels[i];

    // Delete commands
    for (std::map<std::string, Command *>::iterator it = _commands.begin(); it != _commands.end(); it++)
            delete it->second;
}

Server  &Server::operator=(const Server &other)
{
    this->_running = other._running;
    this->_socket = other._socket;
    this->_port = other._port;
    this->_password = other._password;
    this->_hostname = other._hostname;
    this->_oper_password = other._oper_password;
    this->_info = other._info;
    this->_pollfds = other._pollfds;
    this->_clients = other._clients;
    this->_commands = other._commands;
    return *this;
}

//POLLHUP : poll hangup (close connection)
void Server::on_client_connect(void)
{
    sockaddr_in  client_address;
    socklen_t    client_address_size = sizeof(client_address);
    int client_fd = accept(_socket, (struct sockaddr *)&client_address, &client_address_size);

    if (client_fd == -1)
        throw std::runtime_error("Error: can't accept client connection: " + std::string(strerror(errno)));
    addPollfd(_pollfds, client_fd, POLLIN | POLLHUP);

    char hostname[NI_MAXHOST];
    int result = getnameinfo((struct sockaddr *)&client_address, client_address_size, hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);

    if (result != 0)
        throw std::runtime_error(std::string(gai_strerror(result)));

    Client  *client = new Client(_hostname, client_fd, ntohs(client_address.sin_port), _password, hostname);

    _clients.insert(std::make_pair(client_fd, client));
    std::cout << client->get_hostname() << ":" << client->get_port() << " has connected" << std::endl;
}

void Server::on_client_disconnect(int client_fd)
{
    // Remove the client from the map
    std::map<int, Client *>::iterator it = _clients.find(client_fd);
    if (it != _clients.end()) {
        it->second->disconnect("Server disconnected due to inactivity");
        delete it->second;
        _clients.erase(it);
    }

    // Remove the client from the vector
    for (size_t i = 0; i < _pollfds.size(); i++)
    {
        if (_pollfds[i].fd == client_fd) {
            _pollfds.erase(_pollfds.begin() + i);
            break;
        }
    }
}

void Server::client_message(int i, std::string message)
{
    int client_fd = _pollfds[i].fd;
    Client *client;
    std::istringstream iss(message);
    std::string line;

    client = get_client(client_fd);

    //parse from line to line, from the input string stream to the temp line variable
    while (std::getline(iss, line))
    {
        if (client->is_disconnected())
            break ;
        if (line.length() == 0)
            continue ;

        // save the line in a buffer if it does not end in \r
        if (line.length() > 0 && line[line.length() - 1] != '\r')
        {
            client->set_buffer(line);
            continue ;
        }
        try
        {
            if (client->get_buffer().length() > 0)
            {
                line = client->get_buffer() + line;
                client->clear_buffer();
            }
            Message *msg;

            msg = new Message(line);
            std::string cmd = msg->get_command();
            std::cout << cmd << " was called " << std::endl;

            if (_commands.find(cmd) == _commands.end())
                client->reply(ERR_UNKNOWNCOMMAND, ":Unknown command " + cmd);
            else
                _commands[cmd]->invoke(client, msg);
            delete msg;
        } catch(std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

}

Client  *Server::get_client(int client_fd)
{
    std::map<int, Client *>::iterator it = _clients.find(client_fd);
    if (it == _clients.end())
        throw std::runtime_error("Client not found");
    return it->second;
}

Client *Server::get_client(std::string nickname) {
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
        if (it->second->get_nickname() == nickname)
            return it->second;
    return NULL;
}

void   Server::remove_client(std::string nickname)
{
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second->get_nickname() == nickname)
        {
            delete it->second;
            _clients.erase(it);
            return ;
        }
    }
}

//basicamente, esto tiene un loop infinito donde va vigilando, mediante la funcion poll, si en cada fd
// del vector PollFds, ha sucedido el evento que le hemos asignado. le asignamos POLLIN: there is data to read.
// por lo tanto, si hay POLLIN en el socket del servidor, hay un usuario nuevo. si hay POLLIN en stdin,
// tenemos que leer un comando
void Server::event_loop(void)
{
    _running = true;

    addPollfd(_pollfds, _socket, POLLIN);
    addPollfd(_pollfds, STDIN_FILENO, POLLIN);
    while (_running)
    {
        int npolls = poll(_pollfds.data(), _pollfds.size(), 0);

        if (npolls == -1)
            throw std::runtime_error("Error: cannot poll for events: " + std::string(strerror(errno)));

        // If no events happened, continue
        if (npolls == 0)
            continue;

        //server socket
        if (_pollfds[0].revents & POLLIN) {
            try {
                on_client_connect();
            } catch (std::exception &e) {
                throw std::runtime_error("Error: cannot accept client connection: " + std::string(strerror(errno)));
            }
        }

        //standard input
        if (_pollfds[1].revents & POLLIN)
        {
            char buffer[1024];

            int bread = read(STDIN_FILENO, buffer, sizeof(buffer));

            if (bread == -1)
                throw std::runtime_error("Error: while reading from STDIN: " + std::string(strerror(errno)));

            if (bread == 0 || (std::string(buffer, bread) == "exit\n"))
                _running = false;
        }

        //this loops through the pollfds for all the possible clients
        for (int i = 2; i < (int)_pollfds.size(); i++)
        {
            if (_clients.find(_pollfds[i].fd)->second->is_disconnected())
            {
                on_client_disconnect(_pollfds[i].fd);
                continue ;
            }

            // If the client received a HUP event, disconnect it
            if ((_pollfds[i].revents & POLLHUP) == POLLHUP)
                _clients.find(_pollfds[i].fd)->second->disconnect("Client disconnected due to HUP event");
            if ((_pollfds[i].revents & POLLIN) == POLLIN)
            {
                char buffer[1024];
                int bread = recv(_pollfds[i].fd, buffer, sizeof(buffer), 0);

                if (bread == -1)
                    throw std::runtime_error("Error: when receiving data from client: " + std::string(buffer, bread));
                client_message(i, std::string(buffer, bread));
            }
        }
    }
}

//will open the general socket and bind it to arg port.
//TODO: is setting reuse necessary?
void Server::open_general_socket(std::string port)
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1)
        throw std::runtime_error("Error: creating server socket");

    int enable = 1;

    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
        throw std::runtime_error("Error: changing server options");
    if (fcntl(_socket, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Error: changing server socket to nonblock");

    struct sockaddr_in srv_addr = {};

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(std::atoi(port.c_str()));
    srv_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(_socket, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1)
        throw std::runtime_error("Error: binding server socket to port");

    if (listen(_socket, 85) == -1)
        throw std::runtime_error("Error: reading from server socket");
}

void                    Server::add_channel(Channel *channel)
{
    _channels.push_back(channel);
    return ;
}

//getters setters
Channel                 *Server::get_channel(std::string name)
{
    // if name doesn't end in ":hostname", append ":hostname"
    if (name.find(":") == std::string::npos)
        name += ":" + _hostname;
    for (size_t i = 0; i < _channels.size(); i++)
        if (_channels[i]->get_name() == name)
            return _channels[i];
    return NULL;
}

std::string     Server::get_hostname(void)
{
    return _hostname;
}

std::string     Server::get_oper_password(void)
{
    return _oper_password;
}

std::vector<Channel *>  Server::list_channels(void)
{
    return _channels;
}

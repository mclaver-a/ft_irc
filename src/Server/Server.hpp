/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclaver- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 18:40:30 by mclaver-          #+#    #+#             */
/*   Updated: 2025/11/18 18:40:45 by mclaver-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include <string>
#include <unistd.h>
#include <vector>
#include <poll.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <exception>
#include <fcntl.h>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <netdb.h>
# include "../FlexLexer.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

# include "../Message/Message.hpp"
# include "../Client/Client.hpp"
#include "../Commands/Commands.hpp"
#include "../Channel/Channel.hpp"
#include "../Utils/Utils.hpp"

// global variable for op password
const std::string   g_oper_password = "chungus";

class Message;

class Client;

class Command;

class Channel;

class Server
{
    private:
        bool                                _running;
        int                                 _socket;

        std::string                         _port;
        std::string                         _password;
        std::string                         _hostname;
        std::string                         _oper_password;
        std::string                         _info;

        std::vector<pollfd>                 _pollfds;
        std::map<int, Client *>             _clients;
        std::map<std::string, Command *>    _commands;
        std::vector<Channel *>              _channels;
    public:
        Server(std::string port, std::string password);
        Server(const Server &other);
        ~Server();
        Server &operator=(const Server &other);

        void                    on_client_connect(void);
        void                    on_client_disconnect(int client_fd);
        void                    event_loop(void);
        void                    open_general_socket(std::string port);
        void                    client_message(int i, std::string message);
        Client                  *get_client(int client_fd);
        Client                  *get_client(std::string nickname);
        std::string             get_hostname(void);
        std::string             get_oper_password(void);
        std::vector<Channel *>  list_channels(void);
        void                    remove_client(std::string nickname);
        void                    testprint(void);
        void                    add_channel(Channel *channel);
        Channel                 *get_channel(std::string name);

};

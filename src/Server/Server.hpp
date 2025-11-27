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
# include "../Client/Client.hpp"
# include "../FlexLexer.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "../Message/Message.hpp"
#include "../Commands/Commands.hpp"

class Message;

class Client;

class Command;

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
        std::map<std::string, Command *>     _commands;
        //TBD> Clients, channels & commands
    public:
        // Constructors
        Server(std::string port, std::string password);
        Server(const Server &other);
        // Destructor
        ~Server();
        // Assignment operator
        Server &operator=(const Server &other);

        // Awful things we will have to add soon:
/*      void    create_socket(void);
        void    start(void);*/
        void    on_client_connect(void);
        void    event_loop(void);
        void    open_general_socket(std::string port);
        void    client_message(int i, std::string message);
        Client  *get_client(int client_fd);


};

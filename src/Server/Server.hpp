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
#include <sys/socket.h>
#include <netinet/in.h>

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
};

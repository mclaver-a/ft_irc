/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclaver- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2005/11/18 18:40:30 by mclaver-          #+#    #+#             */
/*   Updated: 2005/11/18 18:40:45 by mclaver-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <cstdlib>
# include <cstring>
# include <string>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <stdexcept>
# include <cerrno>
# include <poll.h>
# include <algorithm>
# include <vector>
# include <map>
# include <sstream>

# include "../Utils/utils.hpp"

class Channel;

class Client
{
    private:

    // Attributes
        std::string             _server_hostname;
        int                     _socket;
        int                     _port;
        std::string             _password;

        bool                    _disconnected;
        bool                    _authenticated;
        bool                    _registered;
        bool                    _oper;

        std::string             _nickname;
        std::string             _username;
        std::string             _realname;
        std::string             _hostname;

        std::string             _message_buffer;

    public:
    // Constructors
        Client(std::string server_hostname, int fd, int port, std::string password, const std::string &hostname);
        Client(const Client &other);
    // Destructor
        ~Client();
    // Assignment operator
        Client &operator=(const Client &other);

    // Member functions: Client management
        void        disconnect(std::string message);
        void        authenticate(std::string password);
        void        register_client(void);
        void        oper(std::string oper_password);
        void        unoper(void);

    // Member functions: Message handling
        void        reply(std::string code, std::string message);
        void        broadcast(Client *sender, std::string command, std::string target, std::string message);

    // Getters
        std::string get_server_hostname(void) const;
        int         get_socket(void) const;
        int         get_port(void) const;
        std::string get_nickname(void) const;
        std::string get_username(void) const;
        std::string get_realname(void) const;
        std::string get_hostname(void) const;
        std::string get_buffer(void) const;
        bool        is_disconnected(void) const;
        bool        is_authenticated(void) const;
        bool        is_registered(void) const;
        bool        is_oper(void) const;
        bool        has_nickname(void) const;

    // Setters
        //void        set_nickname(const std::string &nickname);
        void        set_username(const std::string &username);
        void        set_realname(const std::string &realname);
        void        set_nickname(const std::string &nickname);
        void        set_buffer(std::string message);
        void        clear_buffer(void);
};

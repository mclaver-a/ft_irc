/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclaver- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 19:06:15 by mclaver-          #+#    #+#             */
/*   Updated: 2025/11/25 19:06:17 by mclaver-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "../Server/Server.hpp"
# include "../Message/Message.hpp"

class Server;

class Command {
protected:
    std::string _name;
    Server      *_server;

public:
    // Constructors
    Command(std::string name, Server *server);
    Command(const Command &other);
    // Virtual Destructor
    virtual ~Command(void);
    // Assignment operator
    Command &operator=(const Command &other);

    // Member functions
    bool            check_bot_command(std::string message, Channel *channel);
    void            bot_reply(Client *client, Channel *channel, std::string message);
    std::string     get_current_date(void);
    std::string     get_current_time(void);
    std::string     get_random_joke(void);

    virtual void    invoke(Client *client, Message *message) = 0;
};

class Pass : public Command {
public:
    Pass(Server *server);
    ~Pass(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class User : public Command {
public:
    User(Server *server);
    ~User(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class Nick : public Command {
public:
    Nick(Server *server);
    ~Nick(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class Cap : public Command {
public:
    Cap(Server *server);
    ~Cap(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

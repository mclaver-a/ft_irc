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
# include "../Client/Client.hpp"

class Server;

class Client;

class Message;

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
    virtual void    invoke(Client *client, Message *message) = 0;
};

class Pass : public Command {
public:
    Pass(Server *server);
    ~Pass(void);

    void    invoke(Client *client, Message *message);
};

class User : public Command {
public:
    User(Server *server);
    ~User(void);

    void    invoke(Client *client, Message *message);
};

class Nick : public Command {
public:
    Nick(Server *server);
    ~Nick(void);

    void    invoke(Client *client, Message *message);
};

class Cap : public Command {
public:
    Cap(Server *server);
    ~Cap(void);

    void    invoke(Client *client, Message *message);
};

class Join : public Command {
public:
    Join(Server *server);
    ~Join(void);

    void    invoke(Client *client, Message *message);
};

class Topic : public Command {
public:
    Topic(Server *server);
    ~Topic(void);

    void    invoke(Client *client, Message *message);
};

class Oper : public Command {
public:
    Oper(Server *server);
    ~Oper(void);

    void    invoke(Client *client, Message *message);
};

class UnOper : public Command {
public:
    UnOper(Server *server);
    ~UnOper(void);

    void    invoke(Client *client, Message *message);
};

class Mode : public Command {
public:
    Mode(Server *server);
    ~Mode(void);

    void    invoke(Client *client, Message *message);
};

class Quit : public Command {
public:
    Quit(Server *server);
    ~Quit(void);

    void    invoke(Client *client, Message *message);
};

class Who : public Command {
public:
    Who(Server *server);
    ~Who(void);

    void    invoke(Client *client, Message *message);
};

class List : public Command {
public:
    List(Server *server);
    ~List(void);

    void    invoke(Client *client, Message *message);
};

class Invite : public Command {
public:
    Invite(Server *server);
    ~Invite(void);

    void    invoke(Client *client, Message *message);
};

class Kick : public Command {
public:
    Kick(Server *server);
    ~Kick(void);

    void    invoke(Client *client, Message *message);
};

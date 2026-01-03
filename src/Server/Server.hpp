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


# define RPL_TOPIC                  "332" // <channel> :<topic>
# define RPL_NOTOPIC                "331" // <client> <channel> :No topic is set
# define RPL_NAMREPLY               "353" // = <channel> :<nick1> <nick2> <nick3> ...
# define RPL_ENDOFNAMES             "366" // <channel> :End of /NAMES list
# define RPL_YOUREOPER              "381" // <client> :You are now an IRC operator

# define RPL_UMODEIS                "221" // <client> <mode>
# define RPL_ENDOFWHO               "315" // <mask> :End of WHO list
# define RPL_CHANNELMODEIS          "324" // <channel> <mode> <mode params>
# define RPL_WHOREPLY               "352" // <client> <username> <hostname> <servername> <nick> <H|G>[*][@|+] :<hopcount> <realname>
# define RPL_ENDOFMODES             "368" // <client> :End of channel mode list

# define ERR_NOSUCHNICK             "401" // <nickname> :No such nick/channel
# define ERR_NOSUCHSERVER           "402" // <server name> :No such server
# define ERR_NOSUCHCHANNEL          "403" // <channel name> :No such channel
# define ERR_CANNOTSENDTOCHAN       "404" // <channel name> :Cannot send to channel
# define ERR_TOOMANYCHANNELS        "405" // <channel name> :You have joined too many channels
# define ERR_WASNOSUCHNICK          "406" // <nickname> :There was no such nickname
# define ERR_TOOMANYTARGETS         "407" // <target> :Duplicate recipients. No message delivered
# define ERR_UNKNOWNCOMMAND         "421" // <command> :Unknown command
# define ERR_NONICKNAMEGIVEN        "431" // :No nickname given
# define ERR_NICKNAMEINUSE          "433" // <nick> :Nickname is already in use
# define ERR_NOTONCHANNEL           "442" // <client> <channel> :You're not on that channel
# define ERR_ALREADYONCHANNEL       "443" // <channel> :You're already on that channel
# define ERR_NOTREGISTERED          "451" // :You have not registered
# define ERR_NEEDMOREPARAMS         "461" // :Not enough parameters
# define ERR_ALREADYREGISTRED       "462" // :You may not reregister
# define ERR_PASSWDMISMATCH         "464" // :Password incorrect
# define ERR_UNKNOWNMODE            "472" // :Unknown mode char
# define ERR_BADCHANNELKEY          "475" // <channel> :Cannot join channel (+k)
# define ERR_NOPRIVILEGES           "481" // :Permission Denied
# define ERR_CHANOPRIVSNEEDED       "482" // <client> <channel> :You're not channel operator
# define ERR_USERNOTINCHANNEL       "441" // <nick> <channel> :They aren't on that channel
# define ERR_USERONCHANNEL          "443" // <nick> <channel> :is already on channel
# define ERR_INVITEONLYCHAN         "473" // <channel> :Cannot join channel (+i)
# define ERR_BADCHANNELKEY          "475" // <channel> :Cannot join channel (+k)
# define ERR_CHANNELISFULL          "471" // <channel> :Cannot join channel (+l)
# define ERR_NOTOPER                "481" // <nick> :is not an IRC operator
# define ERR_ERRONEUSNICKNAME       "432" // <client> <nick> :Erroneus nickname
# define ERR_ERRONEUSUSERNAME       "467" // <client> <username> :Erroneus username
# define ERR_ERRONEUSREALNAME       "472" // <client> <realname> :Erroneus realname
# define ERR_ALREADYOPER            "485" // <client> :You're already an IRC operator


#include "../Message/Message.hpp"
#include "../Commands/Commands.hpp"
#include "../Channel/Channel.hpp"

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
        void        on_client_connect(void);
        void        event_loop(void);
        void        open_general_socket(std::string port);
        void        client_message(int i, std::string message);
        Client      *get_client(int client_fd);
        Client      *get_client(std::string nickname);
        std::string get_hostname(void);
        std::string get_oper_password(void);

        void        add_channel(Channel *channel);
        Channel     *get_channel(std::string name);

};

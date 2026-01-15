#pragma once

#include <vector>
#include <poll.h>
# include "../../FlexLexer.h"

class Client;
class Channel;

# define RPL_WELCOME                "001" // Welcome to the server!
# define RPL_LISTSTART              "321" // Channel :Users  Name
# define RPL_LIST                   "322" // <channel> <# visible> :<topic>
# define RPL_LISTEND                "323" // :End of /LIST

# define RPL_TOPIC                  "332" // <channel> :<topic>
# define RPL_NOTOPIC                "331" // <client> <channel> :No topic is set
# define RPL_NAMREPLY               "353" // = <channel> :<nick1> <nick2> <nick3> ...
# define RPL_ENDOFNAMES             "366" // <channel> :End of /NAMES list
# define RPL_YOUREOPER              "381" // <client> :You are now an IRC operator

# define RPL_INVITING               "341" // <client> <nick> <channel>

# define RPL_ENDOFWHO               "315" // <mask> :End of WHO list
# define RPL_CHANNELMODEIS          "324" // <channel> <mode> <mode params>
# define RPL_WHOREPLY               "352" // <client> <username> <hostname> <servername> <nick> <H|G>[*][@|+] :<hopcount> <realname>

# define ERR_NOSUCHNICK             "401" // <nickname> :No such nick/channel
# define ERR_NOSUCHCHANNEL          "403" // <channel name> :No such channel
# define ERR_UNKNOWNCOMMAND         "421" // <command> :Unknown command
# define ERR_NONICKNAMEGIVEN        "431" // :No nickname given
# define ERR_ERRONEUSNICKNAME       "432" // <client> <nick> :Erroneus nickname
# define ERR_NICKNAMEINUSE          "433" // <nick> :Nickname is already in use
# define ERR_NOTONCHANNEL           "442" // <client> <channel> :You're not on that channel
# define ERR_NOTREGISTERED          "451" // :You have not registered
# define ERR_NEEDMOREPARAMS         "461" // :Not enough parameters
# define ERR_ALREADYREGISTERED      "462" // :You may not reregister
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
# define ERR_ERRONEUSNICKNAME       "432" // <client> <nick> :Erroneus nickname
# define ERR_ALREADYOPER            "485" // <client> :You're already an IRC operator
# define ERR_NOTOPER                "481" // <nick> :is not an IRC operator

void addPollfd(std::vector<pollfd>& fds, int fd, short events);
std::vector<std::string>    split(std::string message);


#pragma once

#include "../Client/Client.hpp"
#include "../Message/Message.hpp"
#include "../Server/Server.hpp"



class Channel {
    private:
        std::string             _name;
        std::string             _hostname;
        std::string             _topic;
        std::string             _key;
        int                     _user_limit;
        int                     _user_quantity;
        std::vector<Client *>   _clients;
        std::vector<Client *>   _op_clients;
        std::vector<Client *>   _invited_clients;

        bool                    _topic_restriction;
        bool                    _invite_only;
        bool                    _has_key;
        bool                    _has_user_limit;

    public:
        Channel(std::string name, std::string hostname);
        Channel(const Channel &other);
        ~Channel(void);
        Channel &operator=(const Channel &other);

        void                    broadcast(Client *sender, std::string command, std::string target, std::string message);

        // Command functions
        void                    join(Client *client);
        void                    part(Client *client, std::string message);
        void                    quit(Client *client, std::string message);
        void                    topic(Client *client, std::vector<std::string> params);
        void                    names(Client *client);
        void                    mode(Client *client, std::string message);
        void                    kick(Client *client, Client *target, std::string reason);

        // Op management
        void                    add_chanop(Client *client);
        void                    remove_chanop(Client *client);
        bool                    is_chanop(std::string nickname);

        // Channel management
        void                    invite(Client *client);
        void                    leave(Client *client);
        void                    set_mode(char mode, std::vector<std::string> params, Client *client, std::string channel_name);
        void                    unset_mode(char mode, std::vector<std::string> params, Client *client, std::string channel_name);

        // Getters & Setters
        std::string             get_name(void) const;
        std::string             get_topic(void) const;
        std::string             get_key(void) const;
        std::string             get_modes(void);
        int                     get_user_limit(void) const;
        int                     get_user_quantity(void) const;
        Client                  *get_client(std::string nickname, std::vector<Client *> &clients);
        std::vector<Client *>   get_clients(void);
        std::vector<Client *>   get_chanop_clients(void);
        std::string             get_client_names(void);
        std::string             get_chanop_names(void);
        std::string             get_invited_names(void);
        std::string             get_client_info(Client *client, Channel *channel);
        bool                    get_topic_restriction(void) const;
        bool                    get_invite_only(void) const;
        bool                    has_key(void) const;
        bool                    has_user_limit(void) const;
        bool                    has_client(Client *client);

        void                    set_topic(const std::string topic);
        void                    set_bot(bool has_bot);
        void                    increase_user_quantity(void);
};

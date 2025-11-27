/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclaver- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 19:20:52 by mclaver-          #+#    #+#             */
/*   Updated: 2025/11/25 19:22:35 by mclaver-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message(std::string message) {
    try {
        _tokenize(message);
    }
    catch (std::exception &e) {
        throw std::runtime_error("Parse failed: " + std::string(e.what()));
    }
    return ;
}

Message::Message(const Message &other) {
    *this = other;
    return ;
}

Message::~Message(void) {
    return ;
}

Message &Message::operator=(const Message &other) {
    this->_prefix = other._prefix;
    this->_command = other._command;
    this->_params = other._params;
    return *this;
}

std::string Message::get_command(void) const
{
    return (this->_command);
}

std::vector<std::string> Message::get_params(void) const
{
    return (this->_params);
}


void    Message::_tokenize(std::string message) {
    std::vector<std::string> split_msg = split(message);

    if (split_msg.size() < 1) {
        throw std::runtime_error("Message is empty");
    }

    // Check if first token is a prefix and take it out of the vector
    if (split_msg[0][0] == ':') {
        _prefix = split_msg[0].substr(1);
        split_msg.erase(split_msg.begin());
    }

    // The next token is the command, and should be stored in all uppercase
    _command = split_msg[0];
    std::transform(_command.begin(), _command.end(), _command.begin(), ::toupper);

    // The rest of the tokens are parameters
    for (size_t i = 1; i < split_msg.size(); i++) {
        //if the token starts with a colon, everything after it is a single parameter
        if (split_msg[i][0] == ':') {
            std::string param = split_msg[i].substr(1);
            for (size_t j = i + 1; j < split_msg.size(); j++) {
                param += " " + split_msg[j];
            }
            _params.push_back(param);
            break;
        }
        _params.push_back(split_msg[i]);
    }
    return ;
}

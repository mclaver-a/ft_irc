/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mclaver- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 19:06:26 by mclaver-          #+#    #+#             */
/*   Updated: 2025/11/25 19:06:28 by mclaver-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

Command::Command(std::string name, Server *server) : _name(name), _server(server) {
    return ;
}

Command::Command(const Command &other) {
    *this = other;
    return ;
}

Command::~Command(void) {
    return ;
}

Command &Command::operator=(const Command &other) {
    this->_name = other._name;
    this->_server = other._server;
    return *this;
}

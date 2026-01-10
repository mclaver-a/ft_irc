#include "Commands.hpp"

Command::Command(std::string name, Server *server) : _name(name), _server(server) { }

Command::Command(const Command &other) { *this = other; }

Command::~Command(void) { }

Command &Command::operator=(const Command &other)
{
    this->_name = other._name;
    this->_server = other._server;
    return *this;
}

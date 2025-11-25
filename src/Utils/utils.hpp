#pragma once

#include "../Server/Server.hpp"
#  include "../Client/Client.hpp"

void addPollfd(std::vector<pollfd>& fds, int fd, short events);
std::vector<std::string>    split(std::string message);

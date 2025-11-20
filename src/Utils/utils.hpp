#pragma once

#include "../Server/Server.hpp"
#  include "../Client/Client.hpp"

void addPollfd(std::vector<pollfd>& fds, int fd, short events);

#pragma once

#include "../srv/Server.hpp"

void addPollfd(std::vector<pollfd>& fds, int fd, short events);

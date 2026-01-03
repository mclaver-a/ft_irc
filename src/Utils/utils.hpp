#pragma once

#include <vector>
#include <poll.h>
# include "../../FlexLexer.h"

class Client;
class Channel;

void addPollfd(std::vector<pollfd>& fds, int fd, short events);
std::vector<std::string>    split(std::string message);

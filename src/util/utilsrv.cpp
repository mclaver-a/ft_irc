#include "utils.hpp"

//util function to add fds to the pollfds vector, properly config'd.
void addPollfd(std::vector<pollfd>& fds, int fd, short events) {
    pollfd p;
    p.fd = fd;
    p.events = events;
    p.revents = 0;
    fds.push_back(p);
}

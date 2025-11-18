#include <asm-generic/socket.h>
#include <exception>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>

//will open the general socket and bind it to arg port.
//TODO: is password arg necessary?
//TODO: is setting reuse necessary?
//TODO: remove the return logic. when the struct is working we wont need to return anything
int open_general_socket(std::string port, std::string password)
{
    int socket = socket(AF_INET, SOCK_STREAM, 0);
    if (socket == -1)
        throw std::runtime_error("Error: creating server socket");

    int enable = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
        throw std::runtime_error("Error: changing server options");

    if (fcntl(socket, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Error: changing server socket to nonblock");

    struct sockaddr_in srv_addr = {0};
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(std::atoi(port.c_str()));
    srv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1)
        throw std::runtime_error("Error: binding server socket to port");

    if (listen(socket, 85) == -1)
        throw std::runtime_error("Error: reading from server socket");
    return socket;
}

//util function to add fds to the pollfds vector, properly config'd.
void addPollfd(std::vector<pollfd>& fds, int fd, short events) {
    pollfd p;
    p.fd = fd;
    p.events = events;
    p.revents = 0;
    fds.push_back(p);
}

//TODO: toda esta mierda... ahora si que necesito el struct
void on_client_connect(void)
{
    sockaddr_in  client_address;
    socklen_t    client_address_size = sizeof(client_address);
    while (true)
    {
        std::cout << "hi" << std::endl;
        int client_fd = accept(fd, (struct sockaddr *)&client_address, &client_address_size);
        if (client_fd)
        {
            std::cout << "127.0.0.1" << ":" << port << " has connected" << std::endl;
            char buffer[1024] = {0};
            recv(client_fd, buffer, sizeof(buffer), 0);
            std::cout << "Message from Client: " << buffer << std::endl;
            close(client_fd);
        }
    }
}

//basicamente, esto tiene un loop infinito donde va vigilando, mediante la funcion poll, si en cada fd
// del vector PollFds, ha sucedido el evento que le hemos asignado. le asignamos POLLIN: there is data to read.
// por lo tanto, si hay POLLIN en el socket del servidor, hay un usuario nuevo. si hay POLLIN en stdin,
// tenemos que leer un comando
void event_loop(int socket)
{
    bool running = true;

    std::vector<pollfd> pollfds;
    addPollfd(pollfds, socket, POLLIN);
    addPollfd(pollfds, STDIN_FILENO, POLLIN);

    while (running)
    {
        int npolls = poll(pollfds.data(), pollfds.size(), 0);

        if (npolls == -1)
            throw std::runtime_error("Cannot poll for events: " + std::string(strerror(errno)));

        // If no events happened, continue
        if (npolls == 0)
            continue;

        if (pollfds[0].revents & POLLIN) {
            try {
                on_client_connect();
            } catch (std::exception &e) {
                throw std::runtime_error("Cannot accept client connection: " + std::string(strerror(errno)));
            }
        }

        //TODO: mas mierda por aqui...

    }
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <server_password>" << std::endl;
        return 1;
    }
    std::string port = argv[1];
    // Check if port is a number
    if (port.find_first_not_of("0123456789") != std::string::npos) {
        std::cerr << "Error: Port must be a number" << std::endl;
        return 1;
    }
    std::string password = argv[2];

    //TODO: init server struct. the following functions should be member functions
    //TODO: fix this garbage try catch structure
    try
    {
        int socket = open_general_socket(port, password);
        try
        {
            event_loop(socket);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }


}


#include "srv/Server.hpp"



/* MOVER TODO ESTO A Server.cpp
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
*/


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

    Server irc(port, password);
    //TODO: fix this garbage try catch structure
    try
    {
        int socket = irc.open_general_socket(port, password);
        try
        {
            irc.event_loop(socket);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }


}

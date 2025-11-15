#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

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


    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        throw std::runtime_error("Error creating server socket");

    struct sockaddr_in srv_addr = {};
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(std::atoi(port.c_str()));
    srv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1)
        exit(1);

    if (listen(fd, 85) == -1)
        exit(1);

    sockaddr_in  client_address;
    socklen_t    client_address_size = sizeof(client_address);
    //while (1)
    {
        int client_fd = accept(fd, (struct sockaddr *)&client_address, &client_address_size);
        if (client_fd)
        {
            std::cout << "127.0.0.1" << ":" << port << " has connected" << std::endl;
            char buffer[1024];
            int bread = read(STDIN_FILENO, buffer, sizeof(buffer));
            bread += 1;
            std::cout << buffer << std::endl;
        }
    }
}

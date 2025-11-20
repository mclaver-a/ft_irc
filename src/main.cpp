
#include "Server/Server.hpp"

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
        irc.open_general_socket(port);
        try
        {
            irc.event_loop();
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }


}

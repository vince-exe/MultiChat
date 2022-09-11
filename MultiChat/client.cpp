#include "client.h"

Client::Client() {}

bool Client::connect(const std::string& ipAddress, int port) {
    /* allocate space for the socket */
    this->socketClient = new boost::asio::ip::tcp::socket(this->io_context);

    try {
        /* try to create a connection endpoint at the given ip-address and port */
        this->socketClient->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ipAddress), port));
    }
    catch(boost::system::system_error) {
        return false;
    }

    return true;
}

boost::asio::ip::tcp::socket *Client::getSocket() {
    return this->socketClient;
}

void Client::close() {
    this->socketClient->close();
    delete this->socketClient;
}

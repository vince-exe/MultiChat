#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>

class client {
private:
    /* context to execute the work */
    boost::asio::io_context io_context;

    /* socket pointer */
    boost::asio::ip::tcp::socket* socketClient;

public:
    client();

    /* connect to a specific endpoint */
    bool connect(const std::string& ipAddress, int port);
};

#endif // CLIENT_H

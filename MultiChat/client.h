#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>

class Client {
private:
    /* context to execute the work */
    boost::asio::io_context io_context;

    /* socket pointer */
    boost::asio::ip::tcp::socket* socketClient;

public:
    Client();

    /* connect to a specific endpoint */
    bool connect(const std::string& ipAddress, int port);

    /* return the socket pointer */
    boost::asio::ip::tcp::socket *getSocket();

    /* close the socket */
    void close();
};

#endif // CLIENT_H

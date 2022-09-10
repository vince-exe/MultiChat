#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>

class Client {
private:
    /* context to execute the work */
    boost::asio::io_context io_context;

    /* socket pointer */
    boost::asio::ip::tcp::socket* socketClient;

    std::string nickname;

public:
    Client();

    /* connect to a specific endpoint */
    bool connect(const std::string& ipAddress, int port, std::string nickname);

    /* return the socket pointer */
    boost::asio::ip::tcp::socket *getSocket();

    /* close the socket */
    void close();

    std::string getNickname();

};

#endif // CLIENT_H

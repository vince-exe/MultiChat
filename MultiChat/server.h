#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

#include <unordered_map>
#include <vector>
#include <string>

class Server {
private:
    /* context to execute the socket work */
    boost::asio::io_context ioContext;

    /* create an acceptor, for accepting new connections */
    boost::asio::ip::tcp::acceptor* acceptor;

    /* used to store the clients */
    std::unordered_map<std::string, boost::asio::ip::tcp::socket*> clientList;

public:
    Server(std::string ipAddress, int port);

    /* listen for new connections */
    void listen();

    /* save the nickname of the client */
    void pushClientNickname(std::string nickname, const std::string& pos);

    boost::asio::ip::tcp::socket* getSocketAt(const std::string& pos);
};

#endif // SERVER_H

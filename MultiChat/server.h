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

public:
    Server(std::string ipAddress, int port);

    /* listen for new connections */
    void listen();

    /* save the nickname of the client */
    bool pushClientNickname(std::string nickname, const std::string& pos);

    /* get the socket of a specific client */
    boost::asio::ip::tcp::socket* getSocketAt(const std::string& pos);

    /* erase from the client list the given client */
    bool eraseClient(const std::string& pos);

public:
    /* used to store the clients */
    static std::unordered_map<std::string, boost::asio::ip::tcp::socket*> clientList;

    /* get the client list */
    static std::unordered_map<std::string, boost::asio::ip::tcp::socket*> getClientList();

};

#endif // SERVER_H

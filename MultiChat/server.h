#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

#include <map>
#include <vector>
#include <string>

class Server {
private:
    /* context to execute the socket work */
    boost::asio::io_context ioContext;

    /* create an acceptor, for accepting new connections */
    boost::asio::ip::tcp::acceptor* acceptor;

    /* keep trace of the connections */
    int connectionCount = 0;

    /* used to store the clients */
    std::map<std::string, boost::asio::ip::tcp::socket*> clientList;

    std::string ip;

    int port;

    bool isOpen_;

public:
    Server(std::string ipAddress, int port);

    /* listen for new connections */
    void listen();

    /* close the acceptor */
    void stop();

    /* open the acceptor */
    void open();

    /* active the acceptor */
    void activeAcceptor();

    void shutdown();

    bool isOpen() const;

    /* save the nickname of the client */
    void pushClientNickname(std::string nickname, const std::string& pos);

    /* get the socket of a specific client */
    boost::asio::ip::tcp::socket* getSocketAt(const std::string& pos);

    /* erase from the client list the given client */
    bool eraseClient(const std::string& pos);

    /* check if the given user is in the client list */
    bool isClient(const std::string& pos);

    void setConnCount(int count);

    int getConnCount();

    /* serialize the client nicknames list */
    std::string getClientListSerialized(const char* c);

    /* get the client list */
    std::map<std::string, boost::asio::ip::tcp::socket*> getClientList();
};

#endif // SERVER_H

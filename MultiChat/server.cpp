#include "server.h"

Server::Server(std::string ipAddress, int port) {
    /* allocate the space for the acceptor */
    this->acceptor = new boost::asio::ip::tcp::acceptor(this->ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ipAddress), port));
}

void Server::listen() {
    /* temporary allocate the space for a socket to accept the connection */
    this->clientList.insert(std::pair<std::string, boost::asio::ip::tcp::socket*>("", new boost::asio::ip::tcp::socket(this->ioContext)));
    /* accept the connection with the new socket */
    this->acceptor->accept(*this->clientList.at(""));
}

void Server::pushClientNickname(std::string nickname, const std::string& pos) {
    /* extract the node of the new socket to store his nickname */
    auto node = this->clientList.extract(pos);
    node.key() = nickname;

    this->clientList.insert(std::move(node));
}

boost::asio::ip::tcp::socket *Server::getSocketAt(const std::string &pos) {
    return this->clientList.at(pos);
}

bool Server::eraseClient(const std::string &pos) {
    auto it = this->clientList.find(pos);

    /* check if the node exist */
    if(it == this->clientList.end()) {
        return false;
    }

    /* close the socket */
    it->second->close();
    /* dealloc the socket */
    delete it->second;

    /* remove the node from the map */
    this->clientList.erase(it);

    return true;
}

bool Server::isClient(const std::string &pos) {
    return (this->clientList.find(pos) != this->clientList.end());
}

void Server::setConnCount(int count) {
    this->connectionCount = count;
}

int Server::getConnCount() {
    return this->connectionCount;
}

std::string Server::getClientListSerialized(const char *c) {
    std::string str;
    for(auto& it : this->clientList) {
        if(it.first != "") {
            str.append(it.first);
            str.append(c);
        }
    }

    return str;
}

std::map<std::string, boost::asio::ip::tcp::socket *> Server::getClientList() {
    return Server::clientList;
}

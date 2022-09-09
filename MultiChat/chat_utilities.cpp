#include "chat_utilities.h"

std::string ChatUtilities::read_until(boost::asio::ip::tcp::socket* socket, const std::string& del) {
    boost::asio::streambuf buff;
    std::string message;
    boost::system::error_code er;

    boost::asio::read_until(*socket, buff, del, er);
    message = boost::asio::buffer_cast<const char*>(buff.data());

    return message;
}

void ChatUtilities::send(boost::asio::ip::tcp::socket *socket, const std::string &msg) {
    boost::system::error_code er;
    boost::asio::write(*socket, boost::asio::buffer(msg), er);
}

bool ChatUtilities::checkStringSpaces(const QString& string, const char* c, int maxCount) {
    int count = 0;
    for(int i = 0; i < string.length(); i++) {
        if(string[i] == *c) { count++; }
    }
    /* fare più generale */
    return (count <= maxCount);
}

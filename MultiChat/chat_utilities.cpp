#include "chat_utilities.h"

std::string ChatUtilities::read_until(boost::asio::ip::tcp::socket* socket, const std::string& del) {
    boost::asio::streambuf buff;
    std::string message;

    boost::asio::read_until(*socket, buff, del);
    message = boost::asio::buffer_cast<const char*>(buff.data());

    return message;
}

bool ChatUtilities::checkStringCharacter(const QString& string, const char* c, int maxCount) {
    int count = 0;
    for(int i = 0; i < string.length(); i++) {
        if(string[i] == *c) { count++; }
    }
    /* fare più generale */
    return (count <= maxCount);
}

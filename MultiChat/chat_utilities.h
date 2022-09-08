#ifndef CHAT_UTILITIES_H
#define CHAT_UTILITIES_H

#include <boost/asio.hpp>
#include <string>

namespace ChatUtilities {
    /* read message from a socket */
    std::string read_until(boost::asio::ip::tcp::socket* socket, const std::string& del);

    /* send a message to a socket */
    void send(boost::asio::ip::tcp::socket* socket, const std::string& msg);
}

namespace ChatMessages {
    const std::string termCharacter = "\n";

    const std::string nickAlreadyInUse = "[[NickAlreadyInUse]]";

    const std::string clientAccepted = "[[ClientAccepted]]";
}

#endif // CHAT_UTILITIES_H

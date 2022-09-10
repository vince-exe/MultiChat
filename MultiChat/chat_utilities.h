#ifndef CHAT_UTILITIES_H
#define CHAT_UTILITIES_H

#include <boost/asio.hpp>
#include <QString>
#include <string>

namespace ChatUtilities {
    /* read message from a socket */
    std::string read_until(boost::asio::ip::tcp::socket* socket, const std::string& del);

    /* send a message to a socket ( std::string ) */
    void send(boost::asio::ip::tcp::socket* socket, const std::string& msg);

    /* send a message to a socket ( std::vector ) */
    void sendStdVet(boost::asio::ip::tcp::socket* socket, std::vector<std::string>* vec);

    /* count how many times the character appears in the string */
    bool checkStringCharacter(const QString& string, const char* c, int maxCount);

    /* max lenght for a nickname */
    const int maxNickLenght = 16;

    /* min lenght for nickname */
    const int minNickLenght = 5;

    /* max lenght for the port */
    const int minPortLen = 3;
}

namespace ChatMessages {
    const std::string termCharacter = "\n";

    const std::string nickAlreadyInUse = "[.[NickAlreadyInUse].]";

    const std::string clientAccepted = "[.[ClientAccepted].]";

    const std::string connectionTest = "[.[ConnectionTest].]";

    const std::string giveUsersList = "[.[UsersList].]";
}

#endif // CHAT_UTILITIES_H

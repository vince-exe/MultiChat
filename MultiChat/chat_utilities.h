#ifndef CHAT_UTILITIES_H
#define CHAT_UTILITIES_H

#include <boost/asio.hpp>

#include <QString>
#include <string>
#include <QStandardItemModel>

namespace ChatUtilities {
    /* read message from a socket */
    std::string read_until(boost::asio::ip::tcp::socket* socket, const std::string& del);

    /* send a message to a socket ( std::vector ) */
    void sendStdVet(boost::asio::ip::tcp::socket* socket, std::vector<std::string>* vec);

    /* count how many times the character appears in the string */
    bool checkStringCharacter(const QString& string, const char* c, int maxCount);

    /* return an item model */
    QStandardItem *getItem(QString string);

    /* max lenght for a nickname */
    const int maxNickLenght = 16;

    /* min lenght for nickname */
    const int minNickLenght = 5;

    /* max lenght for the port */
    const int minPortLen = 3;
}

namespace ChatMessages {
    const std::string termCharacter = "\n";

    const std::string serializationChar = ";";

    const std::string nickFree = "[.[NickFree].]";

    const std::string connectionTest = "[.[ConnectionTest].]";

    const std::string getClientList = "[.[ClientList].]";

    const std::string clientJoined = "[.[ClientJoined].]";

    const std::string clientListUpdt = "[.[UpdatedClientList].]";
}

#endif // CHAT_UTILITIES_H

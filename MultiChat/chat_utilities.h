#ifndef CHAT_UTILITIES_H
#define CHAT_UTILITIES_H

#include <boost/asio.hpp>

#include <QString>
#include <string>
#include <QStandardItemModel>
#include <QTableView>

namespace ChatUtilities {
    /* read message from a socket */
    std::string read_until(boost::asio::ip::tcp::socket* socket, const std::string& del);

    /* count how many times the character appears in the string */
    bool checkStringCharacter(const QString& string, const char* c, int maxCount);

    /* return an item model */
    QStandardItem *getItem(QString string);

    /* clear the given QTableView from it widgets */
    void clearQTableView(QTableView* table, QStandardItemModel* model, int max);

    const int maxNickLenght = 16;

    const int minNickLenght = 5;

    const int minPortLen = 3;

    const int maxBlackWordLen = 10;

    const int minBlackWordLen = 1;
}

namespace ChatMessages {
    const std::string termCharacter = "\n";

    const std::string serializationChar = ";";

    const std::string nickFree = "[.[NickFree].]";

    const std::string connectionTest = "[.[ConnectionTest].]";

    const std::string checkNickBan = "[.[CheckNickBan].]";

    const std::string clientJoined = "[.[ClientJoined].]";

    const std::string clientListUpdt = "[.[UpdatedClientList].]";

    const std::string clientDisconnect = "[.[ClientDisconnect].]";

    const std::string acceptClientDisconnection = "[.[AcceptClientDisconnection].]";

    const std::string clientLeft = "[.[ClientLeft].]";

    const std::string closeServer = "[.[ServerClose].]";

    const std::string shutdownServer = "[.[ServerShutdown].]";

    const std::string mutedMsg = "[.[Muted].]";

    const std::string unMuteMsg = "[.[UnMute].]";

    const std::string kickMessage = "[.[KickMessage].]";

    const std::string banMessage = "[.[BanMessage].]";

    const std::string blackListMsg = "[.[BlackList].]";
}

#endif // CHAT_UTILITIES_H

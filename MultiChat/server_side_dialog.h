#ifndef SERVER_SIDE_DIALOG_H
#define SERVER_SIDE_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QString>
#include <QTableView>

#include <thread>

#include "server.h"
#include "client_stats_manager.h"

namespace Ui {
class ServerSideDialog;
}

class ServerSideDialog : public QDialog {
    Q_OBJECT

public:
    explicit ServerSideDialog(QWidget *parent = nullptr);
    ~ServerSideDialog();

    QStandardItemModel* modelUsers;
    QStandardItemModel* searchModelUsers;

    QStringList horizontalHeaderUsers;
    QStringList verticalHeaderUsers;

    std::thread acceptThread;

    static Server* server;

    /* vector to store all the active threads */
    std::vector<std::thread> listenClientsThreads;

    /* vector used to store the names of the muted clients */
    static std::vector<std::string> mutedList;

    /* map used to store the ip-addresses and the name of the banned clients */
    static std::map<std::string, std::string> banMap;

    /* map used to store the info of a client */
    static std::map<std::string, ClientStatsManager> statsMap;

    /* vector used to store the banned words */
    static std::vector<std::string> blackWordsVec;

    /* used to store the selected user from the users table */
    static std::string selectedUser;

    static bool isServerOpen;

    static bool guiLoaded;

    static bool serverShutdown;

    static bool checkSearch(std::map<std::string, boost::asio::ip::tcp::socket*> map, const std::string& clientSearched);

    Ui::ServerSideDialog *ui;

public:
    /* return a serialized string that rappresent the banned list */
    std::string serializeBannedList(const char* c);

    /* return a string that contain all the black words serialized */
    std::string serializeBlackWordsList(const char* c);

private slots:
    void on_optionsBtn_clicked();

    void on_searchBtn_clicked();

    void on_resetSearchBtn_clicked();

    void on_searchUserBox_textChanged(const QString &arg1);

    void on_sendMsgBtn_clicked();

    void on_resetMsgBtn_clicked();

    void on_userTable_activated(const QModelIndex &index);

    void on_muteBtn_clicked();

    void on_muteListBtn_clicked();

    void on_kickBtn_clicked();

    void on_banBtn_clicked();

    void on_banListBtn_clicked();

    void on_blackWordsBtn_clicked();

    void on_infoUsrBtn_clicked();

    void on_infoBtn_clicked();

private:
    void reject();

    void keyPressEvent(QKeyEvent* event);
};

/* accept the incoming connections */
void acceptClients(Server* server, ServerSideDialog* objcet);

/* listen the messages of a single client */
void listenClient(const std::string nickname, Server* server, ServerSideDialog* object);

/* function to send the incoming message to all the clients */
void sendToAll(Server* server, const std::string& message, const std::string& nickname);

/* print the client list */
void printClientList(QStandardItemModel* model, std::unordered_map<std::string, boost::asio::ip::tcp::socket*> map, QTableView* table);

/* print the searched client */
void printSearchedClients(const std::string& clientSearched, QStandardItemModel* model, std::unordered_map<std::string, boost::asio::ip::tcp::socket*> map, QTableView* table);

void shutdownServer(ServerSideDialog* object);

#endif // SERVER_SIDE_DIALOG_H

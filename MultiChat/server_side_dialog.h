#ifndef SERVER_SIDE_DIALOG_H
#define SERVER_SIDE_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QString>
#include <QTableView>

#include <thread>

#include "server.h"

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

    /* used to store the selected user from the users table */
    std::string selectedUser;

    static bool isServerOpen;

    static bool guiLoaded;

    static bool serverShutdown;

    Ui::ServerSideDialog *ui;

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

private:
    void reject();
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

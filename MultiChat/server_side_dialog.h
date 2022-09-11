#ifndef SERVER_SIDE_DIALOG_H
#define SERVER_SIDE_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QString>
#include <QTableView>

#include <thread>

#include "server.h"

QStandardItem* getItem(QString string);

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

    /* vector to store all the active threads */
    std::vector<std::thread> listenClientsThreads;

    static bool isServerOpen;

    Ui::ServerSideDialog *ui;

private slots:
    void on_optionsBtn_clicked();

    void on_searchBtn_clicked();

    void on_resetSearchBtn_clicked();

    void on_searchUserBox_textChanged(const QString &arg1);

private:
    void reject();
};

/* accept the incoming connections */
void acceptClients(std::string ip, int port, ServerSideDialog* objcet);

/* listen the messages of a single client */
void listenClient(const std::string nickname, Server& server, ServerSideDialog* object);

void printClientList(QStandardItemModel* model, std::unordered_map<std::string, boost::asio::ip::tcp::socket*> map, QTableView* table);

void printSearchedClients(const std::string& clientSearched, QStandardItemModel* model, std::unordered_map<std::string, boost::asio::ip::tcp::socket*> map, QTableView* table);

#endif // SERVER_SIDE_DIALOG_H

#ifndef SERVER_SIDE_DIALOG_H
#define SERVER_SIDE_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QString>

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

private:

};

/* accept the incoming connections */
void acceptClients(std::string ip, int port, ServerSideDialog* objcet);

/* listen the messages of a single client */
void listenClient(const std::string& nickname, Server& server);

#endif // SERVER_SIDE_DIALOG_H

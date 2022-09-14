#ifndef CLIENT_SIDE_DIALOG_H
#define CLIENT_SIDE_DIALOG_H

#include <QDialog>
#include <thread>
#include <QStandardItemModel>
#include <QTableView>

#include "client.h"

namespace Ui {
class ClientSideDialog;
}

class ClientSideDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientSideDialog(QWidget *parent = nullptr);
    ~ClientSideDialog();

   /* thread to listen the server messages */
   std::thread listenThread;

   /* pointer to the client object */
   Client* client;

   Ui::ClientSideDialog *ui;

   QStandardItemModel* modelUsersClient;

   QStringList horizontalHeaderUsers;
   QStringList verticalHeaderUsers;

   bool isMuted;

private slots:

   void on_sendMsgBtnClient_clicked();

   void on_resetMsgBtnClient_clicked();

private:
    void reject();
};

/* listen the server messages */
void listenServer(ClientSideDialog* object, Client* client);

/* fill the client list */
void fillClientList(std::vector<std::string>* vec, QStandardItemModel* model, QTableView* table);

#endif // CLIENT_SIDE_DIALOG_H

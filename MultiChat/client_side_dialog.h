#ifndef CLIENT_SIDE_DIALOG_H
#define CLIENT_SIDE_DIALOG_H

#include <QDialog>
#include <thread>

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

private slots:
   void on_sendMsgBtn_clicked();

   void on_resetMsgBtn_clicked();

private:
    void reject();
};

/* listen the server messages */
void listenServer(ClientSideDialog* object, Client* client);

#endif // CLIENT_SIDE_DIALOG_H

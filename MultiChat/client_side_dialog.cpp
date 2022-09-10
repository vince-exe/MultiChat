#include "client_side_dialog.h"
#include "ui_client_side_dialog.h"

#include <QMessageBox>

#include "client.h"
#include "ip_port_dialog.h"
#include "chat_utilities.h"
#include "nickname_dialog.h"

void listenServer(ClientSideDialog* object, Client* client) {
    /* send the nickname */
    std::string message;

    /* check if the nickname already exist */
    message = ChatUtilities::read_until(client->getSocket(), ChatMessages::termCharacter);
    message.resize(message.size() - 1);
    qDebug() << QString::fromStdString(message);
    if(message == ChatMessages::nickAlreadyInUse) {
        client->close();
        QMessageBox::critical(0, "Error", "Nickname already in use");
        return;
    }
    /* list of all the users */

    /* start listen the server */
    while(true) {

    }
}

ClientSideDialog::ClientSideDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientSideDialog) {
    ui->setupUi(this);

    /* reduce the borders of the window */
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));

    /* allocate space for the client */
    this->client = new Client();
    /* start the thread to connect at the server */
    this->listenThread = std::thread(listenServer, this, this->client);
}

ClientSideDialog::~ClientSideDialog() {
    delete ui;
}

void ClientSideDialog::on_sendMsgBtn_clicked() {

}

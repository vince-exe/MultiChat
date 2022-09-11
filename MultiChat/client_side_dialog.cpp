#include "client_side_dialog.h"
#include "ui_client_side_dialog.h"

#include <QMessageBox>

#include "client.h"
#include "ip_port_dialog.h"
#include "chat_utilities.h"
#include "nickname_dialog.h"

void listenServer(ClientSideDialog* object, Client* client) {
    /* try to connect */
    if(!client->connect(IpPortDialog::ipAddress, IpPortDialog::port)) {
        QMessageBox::critical(0, "Error", "The server is not responding...");
        return;
    }
    /* send the nickname */
    boost::asio::write(*client->getSocket(), boost::asio::buffer(NicknameDialog::nickname + ChatMessages::termCharacter));

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

/* when the user wants to close the window */
void ClientSideDialog::ClientSideDialog::reject() {
    QDialog::reject();
}

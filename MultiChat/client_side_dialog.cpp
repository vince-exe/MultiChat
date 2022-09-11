#include "client_side_dialog.h"
#include "ui_client_side_dialog.h"

#include <QMessageBox>

#include "client.h"
#include "ip_port_dialog.h"
#include "chat_utilities.h"
#include "nickname_dialog.h"

void fillClientList(std::vector<std::string>* vec, QStandardItemModel* model, QTableView* table) {
    int i = 0;
    for(auto& str : *vec) {
        qDebug() << QString::fromStdString(str);
        model->setItem(i, ChatUtilities::getItem(QString::fromStdString(str)));
        i++;
    }
    table->setModel(model);
}

void listenServer(ClientSideDialog* object, Client* client) {
    /* try to connect */
    if(!client->connect(IpPortDialog::ipAddress, IpPortDialog::port)) {
        QMessageBox::critical(0, "Error", "The server is not responding...");
        return;
    }
    /* send the nickname */
    boost::asio::write(*client->getSocket(), boost::asio::buffer(NicknameDialog::nickname + ChatMessages::termCharacter));

    std::string msg;
    std::vector<std::string> clientList;

    /* list of all the users */ 
    msg = ChatUtilities::read_until(client->getSocket(), ChatMessages::termCharacter);
    clientList = NicknameDialog::deserializeClientList(msg, ChatMessages::serializationChar.c_str());

    fillClientList(&clientList, object->modelUsersClient, object->ui->userTableClient);
    clientList.clear();

    /* start listen the server */
    while(true) {
        msg = ChatUtilities::read_until(client->getSocket(), ChatMessages::termCharacter);
        msg.resize(msg.size() - 1);

        if(msg == ChatMessages::clientJoined) {
            /* request the new client list */
            boost::asio::write(*client->getSocket(), boost::asio::buffer(ChatMessages::clientListUpdt + ChatMessages::termCharacter));

            std::string clientListStr = ChatUtilities::read_until(client->getSocket(), ChatMessages::termCharacter);
            clientListStr.resize(clientListStr.size() - 1);

            clientList = NicknameDialog::deserializeClientList(clientListStr, ChatMessages::serializationChar.c_str());

            fillClientList(&clientList, object->modelUsersClient, object->ui->userTableClient);
            clientList.clear();
            continue;
        }

        /* display the message in the chat */
        QTextCursor textCursor = QTextCursor(object->ui->chatBox->document());
        textCursor.movePosition(QTextCursor::End);
        textCursor.insertText(QString::fromStdString(msg)+ "\n");
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

    /* alloc the QStandardItemModels */
    this->modelUsersClient = new QStandardItemModel();

    this->horizontalHeaderUsers.append("Users List");

    /* set the models */
    this->modelUsersClient->setHorizontalHeaderLabels(this->horizontalHeaderUsers);
    this->modelUsersClient->setVerticalHeaderLabels(this->verticalHeaderUsers);

    /* table settings */
    ui->userTableClient->verticalHeader()->setVisible(false);
    ui->userTableClient->setSelectionMode(QAbstractItemView::NoSelection);
    ui->userTableClient->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->userTableClient->verticalHeader()->setDefaultSectionSize(50);

    ui->userTableClient->setModel(this->modelUsersClient);
}

ClientSideDialog::~ClientSideDialog() {
    delete ui;
}

/* send the message */
void ClientSideDialog::on_sendMsgBtn_clicked() {
    /* check that the message is not empty */
    if(!ui->messageBox->text().length()) {
        return;
    }

    std::string msg = ui->messageBox->text().toStdString();
    /* send the content to the server */
    boost::asio::write(*client->getSocket(), boost::asio::buffer(msg + ChatMessages::termCharacter));

    /* display the content to the screen */
    QTextCursor textCursor = QTextCursor(ui->chatBox->document());
    textCursor.movePosition(QTextCursor::End);
    textCursor.insertText("[ You ] " + QString::fromStdString(msg) + "\n");

    /* clear the text box */
    ui->messageBox->clear();
}

/* when the user wants to close the window */
void ClientSideDialog::ClientSideDialog::reject() {
    QDialog::reject();
}

void ClientSideDialog::on_resetMsgBtn_clicked() {
    ui->messageBox->clear();
}


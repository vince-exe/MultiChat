#include "client_side_dialog.h"
#include "ui_client_side_dialog.h"

#include <QMessageBox>

#include "client.h"
#include "ip_port_dialog.h"
#include "chat_utilities.h"
#include "nickname_dialog.h"

/* forms */
#include "info_user_dialog.h"
#include "server_side_dialog.h"

std::vector<std::string> ClientSideDialog::blackWordsVec;

std::string ClientSideDialog::selectedUser;

void fillClientList(std::vector<std::string>* vec, QStandardItemModel* model, QTableView* table) {
    int i = 0;
    for(auto& str : *vec) {
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
    clientList = NicknameDialog::deserializeList(msg, ChatMessages::serializationChar.c_str());

    fillClientList(&clientList, object->modelUsersClient, object->ui->userTableClient);
    clientList.clear();

    /* list of the black words */
    msg = ChatUtilities::read_until(client->getSocket(), ChatMessages::termCharacter);
    ClientSideDialog::blackWordsVec = NicknameDialog::deserializeList(msg, ChatMessages::serializationChar.c_str());

    /* start listen the server */
    while(true) {
        msg = ChatUtilities::read_until(client->getSocket(), ChatMessages::termCharacter);
        msg.resize(msg.size() - 1);

        if(msg == ChatMessages::clientJoined || msg == ChatMessages::clientLeft) {
            /* request the new client list */
            boost::asio::write(*client->getSocket(), boost::asio::buffer(ChatMessages::clientListUpdt + ChatMessages::termCharacter));

            std::string clientListStr = ChatUtilities::read_until(client->getSocket(), ChatMessages::termCharacter);
            clientListStr.resize(clientListStr.size() - 1);
            clientList = NicknameDialog::deserializeList(clientListStr, ChatMessages::serializationChar.c_str());

            std::string connCount = ChatUtilities::read_until(client->getSocket(), ChatMessages::termCharacter);
            connCount.resize(connCount.size() - 1);

            /* clear the list */
            ChatUtilities::clearQTableView(object->ui->userTableClient, object->modelUsersClient, std::atoi(connCount.c_str())
                                           );
            fillClientList(&clientList, object->modelUsersClient, object->ui->userTableClient);
            clientList.clear();
            continue;
        }

        if(msg == ChatMessages::acceptClientDisconnection) {
            return;
        }

        if(msg == ChatMessages::blackListMsg) {
            /* request the black words list */
            boost::asio::write(*client->getSocket(), boost::asio::buffer(ChatMessages::blackListMsg + ChatMessages::termCharacter));

            std::string blackListStr = ChatUtilities::read_until(client->getSocket(), ChatMessages::termCharacter);
            blackListStr.resize(blackListStr.size() - 1);

            ClientSideDialog::blackWordsVec.clear();
            ClientSideDialog::blackWordsVec = NicknameDialog::deserializeList(blackListStr, ChatMessages::serializationChar.c_str());
            continue;
        }

        if(msg == ChatMessages::kickMessage) {
            boost::asio::write(*client->getSocket(), boost::asio::buffer(ChatMessages::kickMessage + ChatMessages::termCharacter));
            object->isKicked = true;

            /* clear the widgets */
            object->modelUsersClient->clear();
            object->ui->userTableClient->setModel(object->modelUsersClient);

            object->ui->chatBoxClient->clear();

            object->ui->sendMsgBtnClient->setEnabled(false);
            object->ui->resetMsgBtnClient->setEnabled(false);

            object->ui->messageBoxClient->setReadOnly(true);
            object->ui->messageBoxClient->setPlaceholderText("You have been kicked !!");
            return;
        }

        if(msg == ChatMessages::banMessage) {
            boost::asio::write(*client->getSocket(), boost::asio::buffer(ChatMessages::banMessage + ChatMessages::termCharacter));
            object->isBanned = true;

            /* clear the widgets */
            object->modelUsersClient->clear();
            object->ui->userTableClient->setModel(object->modelUsersClient);

            object->ui->chatBoxClient->clear();

            object->ui->sendMsgBtnClient->setEnabled(false);
            object->ui->resetMsgBtnClient->setEnabled(false);

            object->ui->messageBoxClient->setReadOnly(true);
            object->ui->messageBoxClient->setPlaceholderText("You have been banned !!");
            return;
        }

        if(msg == ChatMessages::mutedMsg) {
            object->isMuted = true;
            object->ui->messageBoxClient->setPlaceholderText("You are muted!!");
            object->ui->messageBoxClient->setReadOnly(true);

            object->ui->sendMsgBtnClient->setEnabled(false);
            object->ui->resetMsgBtnClient->setEnabled(false);
            continue;
        }

        if(msg == ChatMessages::unMuteMsg) {
            object->isMuted = false;
            object->ui->messageBoxClient->setPlaceholderText("Send Message");
            object->ui->messageBoxClient->setReadOnly(false);

            object->ui->sendMsgBtnClient->setEnabled(true);
            object->ui->resetMsgBtnClient->setEnabled(true);
            continue;
        }

        if(msg == ChatMessages::shutdownServer) {
            boost::asio::write(*client->getSocket(), boost::asio::buffer(ChatMessages::shutdownServer + ChatMessages::termCharacter));
            client->close();
            object->serverShutdown = true;

            /* clear the widgets */
            object->modelUsersClient->clear();
            object->ui->userTableClient->setModel(object->modelUsersClient);

            object->ui->chatBoxClient->clear();

            object->ui->sendMsgBtnClient->setEnabled(false);
            object->ui->resetMsgBtnClient->setEnabled(false);

            object->ui->messageBoxClient->setReadOnly(true);
            object->ui->messageBoxClient->setPlaceholderText("Server Shutdown !!");
            return;
        }

        /* display the message in the chat */
        QTextCursor textCursor = QTextCursor(object->ui->chatBoxClient->document());
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

    this->isMuted = false;
    this->isKicked = false;
    this->serverShutdown = false;
    this->isBanned = false;
}

ClientSideDialog::~ClientSideDialog() {
    delete ui;
}

bool ClientSideDialog::isBlackWord(std::vector<std::string> *vec, std::string str) {
    int k;
    for(auto& it : *vec ) {
        k = 0;
        for(int i = 0; i < str.length(); i++) {
            if(str[i] == it[k]) { k++; }

            if(k == it.length()) { return true; }
        }
    }
    return false;
}

/* when the user wants to close the window */
void ClientSideDialog::ClientSideDialog::reject() {
    if(this->isKicked || this->serverShutdown || this->isBanned) {
        delete this->client;
        this->close(); return;
    }

    /* check if the user wants to exit */
    QMessageBox confirmBox;
    confirmBox.setText(tr("If you close this window, you will bel disconnected from the chat, are you sure?"));
    confirmBox.addButton(tr("Yes"), QMessageBox::YesRole);

    QAbstractButton* noBtn = confirmBox.addButton(tr("No"), QMessageBox::YesRole);
    /* show the message box */
    confirmBox.exec();
    if(confirmBox.clickedButton() == noBtn) { return; }

    /* send the disconnect message */
    boost::asio::write(*this->client->getSocket(), boost::asio::buffer(ChatMessages::clientDisconnect + ChatMessages::termCharacter));

    /* wait to close the listen thread */
    if(this->listenThread.joinable()) {
        this->listenThread.join();
    }
    /* close the client and dealloc the socket and client pointers*/
    client->close();
    delete this->client;

    QDialog::reject();
}

/* send the message */
void ClientSideDialog::on_sendMsgBtnClient_clicked() {
    if(this->isMuted) { return; }

    /* check that the message is not empty */
    if(!ui->messageBoxClient->text().length()) {
        return;
    }
    /* check that the users doesn't send a message that contains a blackword */
    if(ClientSideDialog::isBlackWord(&ClientSideDialog::blackWordsVec, ui->messageBoxClient->text().toStdString())) {
        ui->messageBoxClient->setStyleSheet("QLineEdit { border: 4px solid '#101014'; font: 700 12pt 'Yu Gothic UI'; letter-spacing: 2px; color: rgb(125, 0, 0); background-color: '#0d3d4c'; padding-left: 13px; } QLineEdit::hover { background-color: '#0c2f3a'; }");
        return;
    }

    std::string msg = ui->messageBoxClient->text().toStdString();
    /* send the content to the server */
    boost::asio::write(*client->getSocket(), boost::asio::buffer(msg + ChatMessages::termCharacter));

    /* display the content to the screen */
    QTextCursor textCursor = QTextCursor(ui->chatBoxClient->document());
    textCursor.movePosition(QTextCursor::End);
    textCursor.insertText("[ You ] " + QString::fromStdString(msg) + "\n");

    /* clear the text box */
    ui->messageBoxClient->clear();
}

void ClientSideDialog::on_resetMsgBtnClient_clicked() {
    ui->messageBoxClient->clear();
}

void ClientSideDialog::keyPressEvent(QKeyEvent *event) {
    /* check if the user pressed ENTER */
    if((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        this->on_sendMsgBtnClient_clicked();
    }
}

void ClientSideDialog::on_messageBoxClient_textChanged(const QString &arg1) {
    ui->messageBoxClient->setStyleSheet("QLineEdit { border: 4px solid '#101014'; font: 700 12pt 'Yu Gothic UI'; letter-spacing: 2px; color: '#e3e3e3'; background-color: '#0d3d4c'; padding-left: 13px; } QLineEdit::hover { background-color: '#0c2f3a'; }");
}

/* Info User */
void ClientSideDialog::on_infoUsrBtnClient_clicked() {
    if(!ServerSideDialog::server->isClient(this->selectedUser)) { return; }

    InfoUserDialog::infoUser = this->selectedUser;

    InfoUserDialog infoUserDialog;
    infoUserDialog.setModal(true);
    infoUserDialog.show();
    infoUserDialog.exec();
}

void ClientSideDialog::on_userTableClient_activated(const QModelIndex &index) {
    QString clientName = index.sibling(index.row(), 0).data().toString();
    this->selectedUser = clientName.toStdString();
}


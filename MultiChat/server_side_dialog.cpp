#include "server_side_dialog.h"
#include "ui_server_side_dialog.h"

#include <QMessageBox>

#include "chat_utilities.h"
#include "server.h"
#include "client.h"

/* forms */
#include "options_server_dialog.h"
#include "ip_port_dialog.h"
#include "mute_list.h"
#include "ban_list_dialog.h"

bool ServerSideDialog::isServerOpen = false;

bool ServerSideDialog::guiLoaded = true;

bool ServerSideDialog::serverShutdown;

Server* ServerSideDialog::server;

std::vector<std::string> ServerSideDialog::mutedList;

std::map<std::string, std::string> ServerSideDialog::banMap;

void printClientList(QStandardItemModel* model, std::map<std::string, boost::asio::ip::tcp::socket*> map, QTableView* table) {
    int i = 0;
    for(auto&it : map) {
        if(it.first.length()) {
            model->setItem(i, ChatUtilities::getItem(QString::fromStdString(it.first)));
            i++;
        }
    }
    table->setModel(model);
}

void printSearchedClients(const std::string &clientSearched, QStandardItemModel *model, std::map<std::string, boost::asio::ip::tcp::socket*> map, QTableView *table) {
    int i = 0;
    for(auto& it : map) {
        if(it.first == clientSearched) {
            model->setItem(i, ChatUtilities::getItem(QString::fromStdString(clientSearched)));
            i++;
        }
    }
    table->setModel(model);
}

void acceptClients(Server* server, ServerSideDialog* object) {
    while(true) {
        /* listen for connection */
        server->listen();

        /* get the nickname from the new socket */
        std::string message = ChatUtilities::read_until(server->getSocketAt(""), ChatMessages::termCharacter);
        /* remove the term character */
        message.resize(message.size() - 1);

        /* check if the server is open */
        if(!server->isOpen()) {
            boost::asio::write(*server->getSocketAt(""), boost::asio::buffer(ChatMessages::closeServer + ChatMessages::termCharacter));
            server->eraseClient("");
            continue;
        }

        if(message == ChatMessages::closeServer) {
            /* stop listening for new connections */
            server->eraseClient("");
            server->stop();
            continue;
        }

        if(message == ChatMessages::shutdownServer) {
            /* warn all the users */
            sendToAll(server, ChatMessages::shutdownServer + ChatMessages::termCharacter, "");
            server->eraseClient("");
            server->shutdown();
            qDebug() << "ok1";
            return;
        }

        if(message == ChatMessages::connectionTest) {
            /* remove the new socket from the map */
            server->eraseClient("");
            continue;
        }

        if(message == ChatMessages::checkNickBan) {
            /* send the list of nicknames */
            boost::asio::write(*server->getSocketAt(""), boost::asio::buffer(server->getClientListSerialized(ChatMessages::serializationChar.c_str()) + ChatMessages::termCharacter));
            /* send the list of the banned ip */

            boost::asio::write(*server->getSocketAt(""), boost::asio::buffer(object->serializeBannedList(ChatMessages::serializationChar.c_str()) + ChatMessages::termCharacter));
            server->eraseClient("");
            continue;
        }

        /* send the welcome message to the other clients */
        sendToAll(server, "[ Server ] " + message + " joined the chat" + ChatMessages::termCharacter, message);
        /* send the joined message, ( when the clients receive this message they automatically request the client list updated */
        sendToAll(server, ChatMessages::clientJoined + ChatMessages::termCharacter, message);

        /* push the new client */
        server->pushClientNickname(message, "");

        /* push the client inside the clients list */
        object->modelUsers->setItem(server->getConnCount(), ChatUtilities::getItem(QString::fromStdString(message)));
        object->ui->userTable->setModel(object->modelUsers);

        /* start the thread to listen the connected client */
        object->listenClientsThreads.push_back(std::thread(listenClient, message, server, object));
        server->setConnCount(server->getConnCount() + 1);
    }
}

void listenClient(const std::string nickname, Server* server, ServerSideDialog* object) {
    /* get the client socket */
    boost::asio::ip::tcp::socket* socketClient = server->getSocketAt(nickname);
    std::string message;

    /* send the client list */
    boost::asio::write(*socketClient, boost::asio::buffer(server->getClientListSerialized(ChatMessages::serializationChar.c_str()) + ChatMessages::termCharacter));

    while(true) {
        /* read the content */
        message = ChatUtilities::read_until(socketClient, ChatMessages::termCharacter);
        message.resize(message.size() - 1);

        if(message == ChatMessages::shutdownServer) {
            server->eraseClient(nickname);
            qDebug() << "ok2";
            return;
        }

        if(message == ChatMessages::clientListUpdt) {
            /* send the client list */
            message = server->getClientListSerialized(ChatMessages::serializationChar.c_str());
            boost::asio::write(*socketClient, boost::asio::buffer(message + ChatMessages::termCharacter));

            /* send the numbers of connected client */
            boost::asio::write(*socketClient, boost::asio::buffer(std::to_string(server->getConnCount()) + ChatMessages::termCharacter));
            continue;
        }

        if(message == ChatMessages::kickMessage) {
            server->eraseClient(nickname);

            sendToAll(server, "[ Server ] The user " + nickname + " has been kicked" + ChatMessages::termCharacter, "");
            sendToAll(server, ChatMessages::clientLeft + ChatMessages::termCharacter, "");

            /* free the table */
            ChatUtilities::clearQTableView(object->ui->userTable, object->modelUsers, server->getConnCount());
            /* reprint the client list */
            printClientList(object->modelUsers, server->getClientList(), object->ui->userTable);
            server->setConnCount(server->getConnCount() - 1);
            return;
        }

        if(message == ChatMessages::banMessage) {
            server->eraseClient(nickname);
            sendToAll(server, "[ Server ] The user " + nickname + " has been banned" + ChatMessages::termCharacter, "");
            sendToAll(server, ChatMessages::clientLeft + ChatMessages::termCharacter, "");

            /* remove the client from the list */
            server->eraseClient(object->selectedUser);

            /* free the table */
            ChatUtilities::clearQTableView(object->ui->userTable, object->modelUsers, server->getConnCount());
            /* reprint the client list */
            printClientList(object->modelUsers, server->getClientList(), object->ui->userTable);
            server->setConnCount(server->getConnCount() - 1);
            return;
        }

        if(message == ChatMessages::clientDisconnect) {
            boost::asio::write(*socketClient, boost::asio::buffer(ChatMessages::acceptClientDisconnection + ChatMessages::termCharacter));
            /* deallocate the socket */
            server->eraseClient(nickname);
            /* free the table */
            ChatUtilities::clearQTableView(object->ui->userTable, object->modelUsers, server->getConnCount());
            /* reprint the client list */
            printClientList(object->modelUsers, server->getClientList(), object->ui->userTable);

            /* update the connection count */
            server->setConnCount(server->getConnCount() - 1);
            /* send to all the users the disconnection message */
            sendToAll(server, "[ Server ] The user " + nickname + " left the chat" + ChatMessages::termCharacter, nickname);
            /* send to all the message that a client left */
            sendToAll(server, ChatMessages::clientLeft + ChatMessages::termCharacter, nickname);
            return;
        }

        /* display the content to the screen */
        QTextCursor textCursor = QTextCursor(object->ui->chatBox->document());
        textCursor.movePosition(QTextCursor::End);
        textCursor.insertText("[ " + QString::fromStdString(nickname) + " ] " + QString::fromStdString(message) + "\n");

        /* send the message to the other clients */
        sendToAll(server, "[ " + nickname + " ] " + message + ChatMessages::termCharacter, nickname);
    }
}

/* function to send the incoming message to all the clients */
void sendToAll(Server* server, const std::string& message, const std::string& nickname) {
    /* iterate over client list */
    for(auto it : server->getClientList()) {
        /* check that message doesn't go at the author and doesn't go at the test socket */
        if(it.first == nickname || it.first == "") { continue; }

        boost::asio::write(*it.second, boost::asio::buffer(message));
    }
}

ServerSideDialog::ServerSideDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerSideDialog) {
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));

    /* alloc the QStandardItemModels */
    this->modelUsers = new QStandardItemModel();
    this->searchModelUsers = new QStandardItemModel();

    this->horizontalHeaderUsers.append("Users List");

    /* set the models */
    this->modelUsers->setHorizontalHeaderLabels(this->horizontalHeaderUsers);
    this->modelUsers->setVerticalHeaderLabels(this->verticalHeaderUsers);

    this->searchModelUsers->setHorizontalHeaderLabels(this->horizontalHeaderUsers);
    this->searchModelUsers->setVerticalHeaderLabels(this->verticalHeaderUsers);

    /* table settings */
    ui->userTable->verticalHeader()->setVisible(false);
    ui->userTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->userTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->userTable->verticalHeader()->setDefaultSectionSize(50);

    ui->userTable->setModel(this->modelUsers);

    ServerSideDialog::isServerOpen = false;

    /* allocate space for the server */
    this->server = new Server(IpPortDialog::ipAddress, IpPortDialog::port);
    this->selectedUser = "..";
}

ServerSideDialog::~ServerSideDialog() {
    delete this->modelUsers;
    delete this->searchModelUsers;

    this->horizontalHeaderUsers.clear();
    this->verticalHeaderUsers.clear();
    delete ui;
}

/* Options Button */
void ServerSideDialog::on_optionsBtn_clicked() {
    OptionsServerDialog optServerDialog;
    optServerDialog.setModal(true);
    optServerDialog.show();
    optServerDialog.exec();

    /* Open Server */
    if(OptionsServerDialog::serverOpened) {
        ServerSideDialog::isServerOpen = true;

        /* activate the acceptor only once */
        if(ServerSideDialog::guiLoaded) {
            this->server->activeAcceptor();
            /* create the thread to listen the clients */
            this->acceptThread = std::thread(acceptClients, this->server, this);
            ServerSideDialog::guiLoaded = !ServerSideDialog::guiLoaded;
        }

        /* open the server */
        this->server->open();
        ui->statusLabel->setStyleSheet("background-color: rgb(6, 86, 1);");
        return;
    }

    /* Close Server */
    if(OptionsServerDialog::wantToCLose) {
        ServerSideDialog::isServerOpen = false;

        /* create a temp client to stop the connection */
        Client client;
        client.connect(IpPortDialog::ipAddress, IpPortDialog::port);
        /* send the close message */
        boost::asio::write(*client.getSocket(), boost::asio::buffer(ChatMessages::closeServer + ChatMessages::termCharacter));
        ui->statusLabel->setStyleSheet("background-color: rgb(125, 0, 0);");

        /* close the client and return */
        client.close();
        return;
    }
}

/* Search Client Button */
void ServerSideDialog::on_searchBtn_clicked() {
    printSearchedClients(ui->searchUserBox->text().toStdString(), this->searchModelUsers, this->server->getClientList(), ui->userTable);
    ui->searchUserBox->clear();
}

/* Reset Client Table Button */
void ServerSideDialog::on_resetSearchBtn_clicked() {
    printClientList(this->modelUsers, this->server->getClientList(), ui->userTable);
    ui->searchUserBox->clear();

}

void ServerSideDialog::on_searchUserBox_textChanged(const QString &arg1) {
    if(arg1.length() > ChatUtilities::maxNickLenght) { ui->searchUserBox->backspace(); }
}

void shutdownServer(ServerSideDialog *object) {
    if(!object->isServerOpen) { return; }

    /* create a temp client to shutdown the connection */
    Client client;

    client.connect(IpPortDialog::ipAddress, IpPortDialog::port);
    boost::asio::write(*client.getSocket(), boost::asio::buffer(ChatMessages::shutdownServer + ChatMessages::termCharacter));
    client.close();

    if(object->acceptThread.joinable()) {
        object->acceptThread.join();
    }

    for(auto& thread : object->listenClientsThreads) {
        if(thread.joinable()) {
            thread.join();
        }
    }
    return;
}

/* when the user wants to close the window */
void ServerSideDialog::ServerSideDialog::reject() {
    QMessageBox confirmBox;

    ServerSideDialog::isServerOpen = true;
    ServerSideDialog::serverShutdown = true;
    this->server->open();

    confirmBox.setText(tr("The application will proceed with shutdown the server, are you sure?"));
    confirmBox.addButton(tr("Yes"), QMessageBox::YesRole);

    QAbstractButton* noBtn = confirmBox.addButton(tr("No"), QMessageBox::YesRole);
    /* show the message box */
    confirmBox.exec();
    if(confirmBox.clickedButton() == noBtn) { return; }

    shutdownServer(this);
    delete this->server;
    this->close();
}

void ServerSideDialog::on_sendMsgBtn_clicked() {
    std::string msg = ui->messageBox->text().toStdString();
    if(!msg.length()) { return; }

    sendToAll(this->server, "[ Server ] " + msg + ChatMessages::termCharacter, "");

    /* display the content to the screen */
    QTextCursor textCursor = QTextCursor(this->ui->chatBox->document());
    textCursor.movePosition(QTextCursor::End);
    textCursor.insertText("[ Server ] " + QString::fromStdString(msg) + "\n");
    this->ui->messageBox->clear();
}

void ServerSideDialog::on_resetMsgBtn_clicked() {
    ui->messageBox->clear();
}

/* save the name of the selected client */
void ServerSideDialog::on_userTable_activated(const QModelIndex &index) {
    QString clientName = index.sibling(index.row(), 0).data().toString();
    this->selectedUser = clientName.toStdString();
}

/* Mute Button */
void ServerSideDialog::on_muteBtn_clicked() {
    if(!this->server->isClient(this->selectedUser)) { return; }

    /* push the user in the muted list */
    this->mutedList.push_back(this->selectedUser);

    /* send the mute message to the client */
    boost::asio::write(*this->server->getSocketAt(this->selectedUser), boost::asio::buffer(ChatMessages::mutedMsg + ChatMessages::termCharacter));

    QMessageBox::information(0, "Success", "Successfully muted the user " + QString::fromStdString(this->selectedUser));
}

/* Show the mute list menu */
void ServerSideDialog::on_muteListBtn_clicked() {
    MuteList muteListDialog;

    muteListDialog.setModal(true);
    muteListDialog.show();
    muteListDialog.exec();
}

/* Key Press event */
void ServerSideDialog::keyPressEvent(QKeyEvent *event) {
    /* check if the user pressed ENTER */
    if((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        this->on_sendMsgBtn_clicked();
    }
}

/* Kick a user */
void ServerSideDialog::on_kickBtn_clicked() {
    if(!this->server->isClient(this->selectedUser)) { return; }

    /* send the kick message */
    boost::asio::write(*this->server->getSocketAt(this->selectedUser), boost::asio::buffer(ChatMessages::kickMessage + ChatMessages::termCharacter));
    QMessageBox::information(0, "Success", "Successfully kicked the user " + QString::fromStdString(this->selectedUser));
}

/* Ban a user */
void ServerSideDialog::on_banBtn_clicked() {
    if(!this->server->isClient(this->selectedUser)) { return; }

    /* send the ban message */
    boost::asio::write(*this->server->getSocketAt(this->selectedUser), boost::asio::buffer(ChatMessages::banMessage + ChatMessages::termCharacter));

    /* get the ip address of the selected user */
    boost::asio::ip::tcp::endpoint remoteEp = this->server->getSocketAt(this->selectedUser)->remote_endpoint();
    boost::asio::ip::address remoteAd = remoteEp.address();

    /* push the user inside the ban map */
    this->banMap.insert(std::pair<std::string, std::string>(this->selectedUser, remoteAd.to_string()));

    QMessageBox::information(0, "Success", "Successfully banned the user " + QString::fromStdString(this->selectedUser));
}

std::string ServerSideDialog::serializeBannedList(const char *c) {
    std::string str;
    for(auto& it : this->banMap) {
        str.append(it.second);
        str.append(c);
    }

    return str;
}

/* Show The Ban List */
void ServerSideDialog::on_banListBtn_clicked() {
    BanListDialog banListDialog;
    banListDialog.setModal(true);
    banListDialog.show();
    banListDialog.exec();
}


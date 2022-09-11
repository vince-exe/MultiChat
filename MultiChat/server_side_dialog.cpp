#include "server_side_dialog.h"
#include "ui_server_side_dialog.h"

#include "chat_utilities.h"
#include "server.h"

/* forms */
#include "options_server_dialog.h"
#include "ip_port_dialog.h"

bool ServerSideDialog::isServerOpen = false;

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
    /* open the server at the given ip and port */

    while(true) {
        /* listen for connection */
        server->listen();
        /* get the nickname from the new socket */
        std::string message = ChatUtilities::read_until(server->getSocketAt(""), ChatMessages::termCharacter);
        /* remove the term character */
        message.resize(message.size() - 1);

        if(message == ChatMessages::connectionTest) {
            /* remove the new socket from the map */
            server->eraseClient("");
            continue;
        }
        if(message == ChatMessages::getClientList) {
            /* send the list of nicknames */
            boost::asio::write(*server->getSocketAt(""), boost::asio::buffer(server->getClientListSerialized(ChatMessages::serializationChar.c_str()) + ChatMessages::termCharacter));
            server->eraseClient("");
            continue;
        }
        /* send the welcome message to the other clients */
        sendToAll(server, "[ Server ] " + message + " joined the chat" + ChatMessages::termCharacter, message);
        /* send the joined message, ( when the clients receive this message they automatically request the client list updated */
        sendToAll(server, ChatMessages::clientJoined + ChatMessages::termCharacter, message);

        /* push the new client */
        server->pushClientNickname(message, "");

        /* send the
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

        if(message == ChatMessages::clientListUpdt) {
            /* send the client list */
            message = server->getClientListSerialized(ChatMessages::serializationChar.c_str());
            boost::asio::write(*socketClient, boost::asio::buffer(message + ChatMessages::termCharacter));
            continue;
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

    /* if the user wants to open the server */
    if(OptionsServerDialog::serverOpened) {
        ServerSideDialog::isServerOpen = true;

        /* create the thread to listen the clients */
        this->acceptThread = std::thread(acceptClients, this->server, this);
        ui->statusLabel->setStyleSheet("background-color: rgb(6, 86, 1);");
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

/* when the user wants to close the window */
void ServerSideDialog::ServerSideDialog::reject() {
    QDialog::reject();
}

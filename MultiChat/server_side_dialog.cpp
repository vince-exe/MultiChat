#include "server_side_dialog.h"
#include "ui_server_side_dialog.h"

#include "chat_utilities.h"
#include "server.h"

/* forms */
#include "options_server_dialog.h"
#include "ip_port_dialog.h"

bool ServerSideDialog::isServerOpen = false;

/* return an item pointer with the text aligned */
QStandardItem* getItem(QString string) {
    QStandardItem* item = new QStandardItem;
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(string);

    return item;
}

void acceptClients(std::string ip, int port, ServerSideDialog* object) {
    /* open the server at the given ip and port */
    Server server(ip, port);

    /* keep the count of the clients */
    int indexClient = 0;

    while(true) {
        /* listen for connection */
        server.listen();
        /* get the nickname from the new socket */
        std::string nickname = ChatUtilities::read_until(server.getSocketAt(""), ChatMessages::termCharacter);
        /* remove the term character */
        nickname.resize(nickname.size() - 1);

        /* try to push the client inside the map of clients */
        if(!server.pushClientNickname(nickname, "")) {
            /* there is already a client with this nickname */
            ChatUtilities::send(server.getSocketAt(""), ChatMessages::nickAlreadyInUse + ChatMessages::termCharacter);
            /* remove the new socket from the map */
            server.eraseClient("");
            continue;
        }
        /* send the accept message */
        ChatUtilities::send(server.getSocketAt(nickname), ChatMessages::clientAccepted + ChatMessages::termCharacter);

        /* push the client inside the clients list */
        object->modelUsers->setItem(indexClient, getItem(QString::fromStdString(nickname)));
        object->ui->userTable->setModel(object->modelUsers);

        /* start the thread to listen the connected client */
        object->listenClientsThreads.push_back(std::thread(listenClient, std::ref(nickname), std::ref(server)));
        indexClient++;
    }
}

void listenClient(const std::string& nickname, Server& server) {
    /* get the client socket */
    boost::asio::ip::tcp::socket* socketClient = server.getSocketAt(nickname);
    std::string message;

    while(true) {
        /* read the content */
        message = ChatUtilities::read_until(socketClient, ChatMessages::termCharacter);
        qDebug() << QString::fromStdString(message);
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
        this->acceptThread = std::thread(acceptClients, IpPortDialog::ipAddress, IpPortDialog::port, this);
        ui->statusLabel->setStyleSheet("background-color: rgb(6, 86, 1);");
        return;
    }
}

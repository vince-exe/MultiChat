#include "server_side_dialog.h"
#include "ui_server_side_dialog.h"

#include "chat_utilities.h"
#include "server.h"

/* return an item pointer with the text aligned */
QStandardItem* getItem(QString string) {
    QStandardItem* item = new QStandardItem;
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(string);

    return item;
}

void listen_clients() {
    Server server("127.0.0.1", 8000);

    while(true) {
        /* listen for connection */
        server.listen();

        std::string nickname = ChatUtilities::read_until(server.getSocketAt(""), "\n");

        qDebug() << QString::fromStdString(nickname);

        /* push the client inside the map of clients */
        server.pushClientNickname(nickname, "");
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
    ui->userTable->verticalHeader()->setDefaultSectionSize(55);

    ui->userTable->setModel(this->modelUsers);
}

ServerSideDialog::~ServerSideDialog() {
    delete this->modelUsers;
    delete this->searchModelUsers;

    this->horizontalHeaderUsers.clear();
    this->verticalHeaderUsers.clear();
    delete ui;
}

void ServerSideDialog::on_pushButton_clicked() {
    this->listen_thread = std::thread(listen_clients);
}

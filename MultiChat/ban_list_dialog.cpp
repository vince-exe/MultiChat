#include "ban_list_dialog.h"
#include "ui_ban_list_dialog.h"

#include <QModelIndex>
#include <QMessageBox>

#include "chat_utilities.h"
#include "server_side_dialog.h"

BanListDialog::BanListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BanListDialog) {
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));

    /* alloc the QStandardItemModels */
    this->banModel = new QStandardItemModel();
    this->horizontalHeaderUsers.append("Muted Users");

    /* set the models */
    this->banModel->setHorizontalHeaderLabels(this->horizontalHeaderUsers);
    this->banModel->setVerticalHeaderLabels(this->verticalHeaderUsers);

    /* table settings */
    ui->banTable->verticalHeader()->setVisible(false);
    ui->banTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->banTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->banTable->verticalHeader()->setDefaultSectionSize(50);

    ui->banTable->setModel(this->banModel);
    this->selectedUser = "..";

    /* print the list of the users */
    this->printBanList(&ServerSideDialog::banMap);
}

BanListDialog::~BanListDialog() {
    delete ui;
}

void BanListDialog::printBanList(std::map<std::string, std::string>* banMap) {
    int i = 0;
    for(auto& it : *banMap) {
        this->banModel->setItem(i, ChatUtilities::getItem(it.first.c_str()));
        i++;
    }
    ui->banTable->setModel(this->banModel);
}

void BanListDialog::on_banTable_activated(const QModelIndex &index) {
    QString clientName = index.sibling(index.row(), 0).data().toString();
    this->selectedUser = clientName.toStdString();
}

void BanListDialog::on_doneBtn_clicked() {
    this->close();
}

/* Pardon a User */
void BanListDialog::on_pardonBtn_clicked() {
    if(ServerSideDialog::banMap.find(this->selectedUser) == ServerSideDialog::banMap.end()) { return; }

    /* delete the user from the list */
    ServerSideDialog::banMap.erase(ServerSideDialog::banMap.find(this->selectedUser));

    /* clear and reprint the table */
    ChatUtilities::clearQTableView(ui->banTable, this->banModel, ServerSideDialog::banMap.size() + 1);
    this->printBanList(&ServerSideDialog::banMap);

    QMessageBox::information(0, "Success", "Successfully pardoned the user " + QString::fromStdString(this->selectedUser));
}

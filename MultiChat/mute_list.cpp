#include "mute_list.h"
#include "ui_mute_list.h"

#include "server_side_dialog.h"
#include "chat_utilities.h"

#include <boost/asio.hpp>
#include <QModelIndex>
#include <QMessageBox>

MuteList::MuteList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MuteList) {

    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));

    /* alloc the QStandardItemModels */
    this->mutedModel = new QStandardItemModel();
    this->horizontalHeaderUsers.append("Muted Users");

    /* set the models */
    this->mutedModel->setHorizontalHeaderLabels(this->horizontalHeaderUsers);
    this->mutedModel->setVerticalHeaderLabels(this->verticalHeaderUsers);

    /* table settings */
    ui->mutedTable->verticalHeader()->setVisible(false);
    ui->mutedTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->mutedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->mutedTable->verticalHeader()->setDefaultSectionSize(50);

    ui->mutedTable->setModel(this->mutedModel);
    this->selectedUser = "..";

    /* print the list of the users */
    this->printMutedList(&ServerSideDialog::mutedList);
}

MuteList::~MuteList() {
    delete this->mutedModel;

    this->horizontalHeaderUsers.clear();
    this->verticalHeaderUsers.clear();
    delete ui;
}

void MuteList::printMutedList(std::vector<std::string> *vec) {
    int i = 0;
    for(auto& it : *vec) {
        this->mutedModel->setItem(i, ChatUtilities::getItem(it.c_str()));
        i++;
    }
    ui->mutedTable->setModel(this->mutedModel);
}

/* Un-Mute button */
void MuteList::on_unMuteBtn_clicked() {
    if(!ServerSideDialog::server->isClient(this->selectedUser)) { return; }

    auto& it = ServerSideDialog::statsMap.at(this->selectedUser);
    it.setMuted(false);

    /* send the un-mute message */
    boost::asio::write(*ServerSideDialog::server->getSocketAt(this->selectedUser), boost::asio::buffer(ChatMessages::unMuteMsg + ChatMessages::termCharacter));
    /* erase the element from the muted list */
    ServerSideDialog::mutedList.erase(std::remove(ServerSideDialog::mutedList.begin(), ServerSideDialog::mutedList.end(), this->selectedUser));

    /* clear the table */
    ChatUtilities::clearQTableView(ui->mutedTable, this->mutedModel, ServerSideDialog::mutedList.size() + 1);
    /* print the list of the users */
    this->printMutedList(&ServerSideDialog::mutedList);

    QMessageBox::information(0, "Success", "Successfully un-muted the user " + QString::fromStdString(this->selectedUser));
}

/* Done Button */
void MuteList::on_doneBtn_clicked() {
    this->close();
}

void MuteList::on_mutedTable_activated(const QModelIndex &index) {
    QString clientName = index.sibling(index.row(), 0).data().toString();
    this->selectedUser = clientName.toStdString();
}

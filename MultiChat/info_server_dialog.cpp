#include "info_server_dialog.h"
#include "ui_info_server_dialog.h"

#include "ip_port_dialog.h"
#include "server_side_dialog.h"

InfoServerDialog::InfoServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoServerDialog) {
    ui->setupUi(this);

    /* reduce the borders of the window */
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));

    ui->ipTextBox->setText(QString::fromStdString(IpPortDialog::ipAddress));
    ui->portTextBox->setText(QString::number(IpPortDialog::port));

    ui->usersCountBox->setText(QString::number(ServerSideDialog::server->getConnCount()));
    ui->mutedUsersBox->setText(QString::number(ServerSideDialog::mutedList.size()));
    ui->bannedUsersBox->setText(QString::number(ServerSideDialog::banMap.size()));

    int countMessage = 0;
    for(auto& it : ServerSideDialog::statsMap) {
        countMessage += it.second.getMsgs();
    }
    ui->messagesBox->setText(QString::number(countMessage));
}

InfoServerDialog::~InfoServerDialog() {
    delete ui;
}

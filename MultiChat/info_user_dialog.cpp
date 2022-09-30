#include "info_user_dialog.h"
#include "ui_info_user_dialog.h"

#include "server_side_dialog.h"

InfoUserDialog::InfoUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoUserDialog) {
    ui->setupUi(this);

    /* reduce the borders of the window */
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));

    auto& clientStats = ServerSideDialog::statsMap.at(ServerSideDialog::selectedUser);

    /* fill the boxes with the user informations */
    if(clientStats.isMuted()) {
        ui->mutedLabel->setStyleSheet("background-color: rgb(6, 86, 1);");
    }
    else {
        ui->mutedLabel->setStyleSheet("background-color: rgb(125, 0, 0);");
    }

    ui->nicknameBox->setText(QString::fromStdString(ServerSideDialog::selectedUser));
    ui->mutesBox->setText(QString::number(clientStats.getMutes()));
    ui->msgBox->setText(QString::number(clientStats.getMsgs()));
 }

InfoUserDialog::~InfoUserDialog() {
    delete ui;
}

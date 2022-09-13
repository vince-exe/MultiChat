#include "options_server_dialog.h"
#include "ui_options_server_dialog.h"

#include "server_side_dialog.h"

#include <QMessageBox>

bool OptionsServerDialog::serverOpened;

bool OptionsServerDialog::wantToCLose;

bool OptionsServerDialog::wantShutdown;

OptionsServerDialog::OptionsServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsServerDialog) {
    ui->setupUi(this);

    OptionsServerDialog::serverOpened = false;
    OptionsServerDialog::wantToCLose = false;
    OptionsServerDialog::wantShutdown = false;
}

OptionsServerDialog::~OptionsServerDialog() {
    delete ui;
}

void OptionsServerDialog::on_startServerBtn_clicked() {
    if(ServerSideDialog::isServerOpen) {
        QMessageBox::warning(0, "Warning", "The server is already open");
        OptionsServerDialog::serverOpened = false;
        return;
    }

    QMessageBox::information(0, "Success", "Successfully opened the server");
    OptionsServerDialog::serverOpened = true;
    this->close();
}

void OptionsServerDialog::on_closeServerBtn_clicked() {
    if(!ServerSideDialog::isServerOpen) {
        QMessageBox::warning(0, "Warning", "The server is already closed");
        OptionsServerDialog::serverOpened = false;
        return;
    }

    QMessageBox::information(0, "Success", "Successfully closed the server");
    OptionsServerDialog::wantToCLose = true;
    this->close();
}

void OptionsServerDialog::on_backupChatBtn_clicked() {

}

void OptionsServerDialog::on_backupUsersBtn_clicked() {

}

/* shutdown the server */
void OptionsServerDialog::on_shutdownBtn_clicked() {
    if(!ServerSideDialog::isServerOpen) {
        QMessageBox::warning(0, "Warning", "Can't shutdown a closed server");
        OptionsServerDialog::serverOpened = false;
        return;
    }

    QMessageBox confirmBox;
    confirmBox.setText(tr("The application will proceed with shutdown the server, are you sure?"));
    confirmBox.addButton(tr("Yes"), QMessageBox::YesRole);

    QAbstractButton* noBtn = confirmBox.addButton(tr("No"), QMessageBox::YesRole);
    /* show the message box */
    confirmBox.exec();
    if(confirmBox.clickedButton() == noBtn) { return; }

    QMessageBox::information(0, "Success", "Successfully shutdown the server");
    OptionsServerDialog::wantShutdown = true;
    this->close();
}

#include "options_server_dialog.h"
#include "ui_options_server_dialog.h"

#include "server_side_dialog.h"

#include <QMessageBox>

bool OptionsServerDialog::serverOpened;

bool OptionsServerDialog::wantToCLose;

OptionsServerDialog::OptionsServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsServerDialog) {
    ui->setupUi(this);

    OptionsServerDialog::serverOpened = false;
    OptionsServerDialog::wantToCLose = false;
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

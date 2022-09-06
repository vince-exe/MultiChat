#include "server_side_dialog.h"
#include "ui_server_side_dialog.h"

ServerSideDialog::ServerSideDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerSideDialog) {
    ui->setupUi(this);
}

ServerSideDialog::~ServerSideDialog() {
    delete ui;
}

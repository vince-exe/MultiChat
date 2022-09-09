#include "client_side_dialog.h"
#include "ui_client_side_dialog.h"

ClientSideDialog::ClientSideDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientSideDialog) {
    ui->setupUi(this);

    /* reduce the borders of the window */
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));
}

ClientSideDialog::~ClientSideDialog() {
    delete ui;
}

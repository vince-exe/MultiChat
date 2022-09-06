#include "main_dialog.h"
#include "ui_main_dialog.h"

/* forms */
#include "ip_port_dialog.h"

bool MainDialog::enterAsServer = false;

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainDialog) {
    ui->setupUi(this);

    /* reduce the borders of the window */
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));

    /* align the items */
    ui->titleLabel->setAlignment(Qt::AlignCenter);
    ui->lineLabel->setAlignment(Qt::AlignCenter);
    ui->copyrightLabel->setAlignment(Qt::AlignCenter);
}

MainDialog::~MainDialog() {
    delete ui;
}

void MainDialog::on_createBtn_clicked() {
    IpPortDialog ipPortDialog;
    MainDialog::enterAsServer = true;

    ipPortDialog.setModal(true);
    ipPortDialog.show();
    ipPortDialog.exec();

    if(!IpPortDialog::doneBtnPressed) { return; }
}

void MainDialog::on_connectBtn_clicked() {
    IpPortDialog ipPortDialog;
    MainDialog::enterAsServer = false;

    ipPortDialog.setModal(true);
    ipPortDialog.show();
    ipPortDialog.exec();

    if(!IpPortDialog::doneBtnPressed) { return; }
}


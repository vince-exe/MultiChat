#include "main_dialog.h"
#include "ui_main_dialog.h"

/* forms */
#include "ip_port_dialog.h"
#include "server_side_dialog.h"
#include "nickname_dialog.h"
#include "client_side_dialog.h"

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
    IpPortDialog::enterAsServer = true;
    IpPortDialog ipPortDialog;

    ipPortDialog.setModal(true);
    ipPortDialog.show();
    ipPortDialog.exec();

    if(!IpPortDialog::doneBtnPressed) { return; }

    ServerSideDialog serverSideDialog;

    serverSideDialog.show();
    serverSideDialog.exec();

    if(ServerSideDialog::serverShutdown) {
        this->close(); return;
    }
}

void MainDialog::on_connectBtn_clicked() {
    IpPortDialog::enterAsServer = false;
    IpPortDialog ipPortDialog;

    ipPortDialog.setModal(true);
    ipPortDialog.show();
    ipPortDialog.exec();

    if(!IpPortDialog::doneBtnPressed) { return; }

    NicknameDialog nickDialog;
    nickDialog.setModal(true);
    nickDialog.show();
    nickDialog.exec();

    if(!NicknameDialog::insertNickname) { return; }

    ClientSideDialog clientSideDialog;

    clientSideDialog.show();
    clientSideDialog.exec();
}


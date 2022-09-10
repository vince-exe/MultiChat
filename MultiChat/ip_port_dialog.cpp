#include "ip_port_dialog.h"
#include "ui_ip_port_dialog.h"

#include <QMessageBox>

#include "chat_utilities.h"
#include "client.h"

#include <thread>

bool IpPortDialog::doneBtnPressed = false;

int IpPortDialog::port;

std::string IpPortDialog::ipAddress;

bool IpPortDialog::enterAsServer;

IpPortDialog::IpPortDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpPortDialog) {
    ui->setupUi(this);

    /* reduce the borders of the window */
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));
    /* align the items */
    ui->titleLabel->setAlignment(Qt::AlignCenter);
    ui->lineLabel->setAlignment(Qt::AlignCenter);

    IpPortDialog::doneBtnPressed = false;
}

IpPortDialog::~IpPortDialog() {
    delete ui;
}

void IpPortDialog::on_portLineEdit_textChanged(const QString &arg1) {
    bool ok;

    arg1.toInt(&ok, 10);
    /* check if the user entered a letter */
    if(!ok) { ui->portLineEdit->backspace(); }

    if(arg1.length() > IpPortDialog::maxPortLen) { ui->portLineEdit->backspace(); }
}

void IpPortDialog::on_ipLineEdit_textChanged(const QString &arg1) {
    if(arg1.length() > IpPortDialog::maxIpLen) { ui->ipLineEdit->backspace(); }
}

void IpPortDialog::on_cancelBtn_clicked() {
    IpPortDialog::doneBtnPressed = false;
    this->close();
}

void IpPortDialog::on_doneBtn_clicked() {
    if(!ui->ipLineEdit->text().length() || ui->portLineEdit->text().length() <= ChatUtilities::minPortLen) {
        QMessageBox::warning(0, "Warning", "Invalid ip-address / port");
        return;
    }
    IpPortDialog::port = ui->portLineEdit->text().toInt();
    IpPortDialog::ipAddress = ui->ipLineEdit->text().toStdString();

    if(IpPortDialog::enterAsServer) {
        IpPortDialog::doneBtnPressed = true;
    }
    else {
        /* create a "test client" to verify the connection */
        Client client;
        /* connect to the server */
        if(!client.connect(IpPortDialog::ipAddress, IpPortDialog::port, "TEST")) {
            QMessageBox::critical(0, "Error", "The server is not responding...");
            client.close();
            return;
        }
        ChatUtilities::send(client.getSocket(), ChatMessages::connectionTest + ChatMessages::termCharacter);
        client.close();
        IpPortDialog::doneBtnPressed = true;
    }

    this->close(); return;
}

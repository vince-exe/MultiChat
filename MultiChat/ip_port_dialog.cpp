#include "ip_port_dialog.h"
#include "ui_ip_port_dialog.h"

#include <QMessageBox>

#include "main_dialog.h"

#include <thread>

bool IpPortDialog::doneBtnPressed = false;

int IpPortDialog::port;

std::string IpPortDialog::ipAddress;

void testListening(boost::asio::ip::tcp::acceptor& acceptor, boost::asio::ip::tcp::socket& socket_server) {
    try {
        acceptor.accept(socket_server);
    }
    catch(boost::wrapexcept<boost::system::system_error>) {
        std::terminate();
        return;
    }
}

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
    if(!ui->ipLineEdit->text().length() || ui->portLineEdit->text().length() <= 3) {
        QMessageBox::warning(0, "Warning", "Invalid ip-address / port");
        return;
    }
    IpPortDialog::port = ui->portLineEdit->text().toInt();
    IpPortDialog::ipAddress = ui->ipLineEdit->text().toStdString();

    /* Server Side */
    if(MainDialog::enterAsServer) {
        ;
    }
    /* Client Side */
    else {
        ;
    }

    return;
}

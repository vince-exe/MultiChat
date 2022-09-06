#ifndef IP_PORT_DIALOG_H
#define IP_PORT_DIALOG_H

#include <QDialog>
#include <boost/asio.hpp>

void testListening(boost::asio::ip::tcp::acceptor& acceptor, boost::asio::ip::tcp::socket& socket_server);

namespace Ui {
class IpPortDialog;
}

class IpPortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IpPortDialog(QWidget *parent = nullptr);

    static const int maxPortLen = 5;

    static const int maxIpLen = 15;

    static int port;

    static std::string ipAddress;

    static bool doneBtnPressed;

    ~IpPortDialog();

private slots:
    void on_portLineEdit_textChanged(const QString &arg1);

    void on_ipLineEdit_textChanged(const QString &arg1);

    void on_cancelBtn_clicked();

    void on_doneBtn_clicked();

private:
    Ui::IpPortDialog *ui;
};

#endif // IP_PORT_DIALOG_H

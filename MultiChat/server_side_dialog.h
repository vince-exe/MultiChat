#ifndef SERVER_SIDE_DIALOG_H
#define SERVER_SIDE_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QString>

#include <thread>

QStandardItem* getItem(QString string);

void listen_clients(std::string ip, int port);

namespace Ui {
class ServerSideDialog;
}

class ServerSideDialog : public QDialog {
    Q_OBJECT

public:
    explicit ServerSideDialog(QWidget *parent = nullptr);
    ~ServerSideDialog();

    QStandardItemModel* modelUsers;
    QStandardItemModel* searchModelUsers;

    QStringList horizontalHeaderUsers;
    QStringList verticalHeaderUsers;

    std::thread listen_thread;

    static bool isServerOpen;

private slots:
    void on_optionsBtn_clicked();

private:
    Ui::ServerSideDialog *ui;
};

#endif // SERVER_SIDE_DIALOG_H

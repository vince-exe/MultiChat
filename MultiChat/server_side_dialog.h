#ifndef SERVER_SIDE_DIALOG_H
#define SERVER_SIDE_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QString>

#include <thread>

QStandardItem* getItem(QString string);

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

    std::thread acceptThread;

    static bool isServerOpen;

    Ui::ServerSideDialog *ui;

private slots:
    void on_optionsBtn_clicked();

private:

};

/* accept the incoming connections */
void acceptClients(std::string ip, int port, ServerSideDialog* objcet);

/* listen the messages of a single client */
void listenClient(const std::string& nickname);

#endif // SERVER_SIDE_DIALOG_H

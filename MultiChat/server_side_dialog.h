#ifndef SERVER_SIDE_DIALOG_H
#define SERVER_SIDE_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QString>

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

private:
    Ui::ServerSideDialog *ui;
};

#endif // SERVER_SIDE_DIALOG_H

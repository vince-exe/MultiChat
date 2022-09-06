#ifndef SERVER_SIDE_DIALOG_H
#define SERVER_SIDE_DIALOG_H

#include <QDialog>

namespace Ui {
class ServerSideDialog;
}

class ServerSideDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerSideDialog(QWidget *parent = nullptr);
    ~ServerSideDialog();

private:
    Ui::ServerSideDialog *ui;
};

#endif // SERVER_SIDE_DIALOG_H

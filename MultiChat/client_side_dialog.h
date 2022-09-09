#ifndef CLIENT_SIDE_DIALOG_H
#define CLIENT_SIDE_DIALOG_H

#include <QDialog>

namespace Ui {
class ClientSideDialog;
}

class ClientSideDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientSideDialog(QWidget *parent = nullptr);
    ~ClientSideDialog();

private:
    Ui::ClientSideDialog *ui;
};

#endif // CLIENT_SIDE_DIALOG_H

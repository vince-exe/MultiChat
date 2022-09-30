#ifndef INFO_USER_DIALOG_H
#define INFO_USER_DIALOG_H

#include <QDialog>

namespace Ui {
class InfoUserDialog;
}

class InfoUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InfoUserDialog(QWidget *parent = nullptr);
    ~InfoUserDialog();

private:
    Ui::InfoUserDialog *ui;
};

#endif // INFO_USER_DIALOG_H

#ifndef INFO_SERVER_DIALOG_H
#define INFO_SERVER_DIALOG_H

#include <QDialog>

namespace Ui {
class InfoServerDialog;
}

class InfoServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InfoServerDialog(QWidget *parent = nullptr);
    ~InfoServerDialog();

private:
    Ui::InfoServerDialog *ui;
};

#endif // INFO_SERVER_DIALOG_H

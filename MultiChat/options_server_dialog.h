#ifndef OPTIONS_SERVER_DIALOG_H
#define OPTIONS_SERVER_DIALOG_H

#include <QDialog>

namespace Ui {
class OptionsServerDialog;
}

class OptionsServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsServerDialog(QWidget *parent = nullptr);
    ~OptionsServerDialog();

    static bool serverOpened;

    static bool wantToCLose;

private slots:
    void on_startServerBtn_clicked();

    void on_closeServerBtn_clicked();

    void on_backupChatBtn_clicked();

    void on_backupUsersBtn_clicked();

private:
    Ui::OptionsServerDialog *ui;
};

#endif // OPTIONS_SERVER_DIALOG_H

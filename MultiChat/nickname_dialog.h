#ifndef NICKNAME_DIALOG_H
#define NICKNAME_DIALOG_H

#include <QDialog>

#include <string>

namespace Ui {
class NicknameDialog;
}

class NicknameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NicknameDialog(QWidget *parent = nullptr);
    ~NicknameDialog();

    static std::string nickname;

    static bool insertNickname;

private slots:
    void on_cancelBtn_clicked();

    void on_doneBtn_clicked();

    void on_nickBox_textChanged(const QString &arg1);

private:
    Ui::NicknameDialog *ui;
};

#endif // NICKNAME_DIALOG_H

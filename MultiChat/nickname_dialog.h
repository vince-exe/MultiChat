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

    static bool checkNickName(std::vector<std::string>* vec, std::string& str);

    static std::vector<std::string> deserializeClientList(const std::string& str, const char* dC);

    static bool nicknameExist(std::vector<std::string>* vec, const std::string& nick);

private slots:
    void on_cancelBtn_clicked();

    void on_doneBtn_clicked();

    void on_nickBox_textChanged(const QString &arg1);

private:
    Ui::NicknameDialog *ui;
};

#endif // NICKNAME_DIALOG_H

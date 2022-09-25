#ifndef BLACK_WORD_DIALOG_HELPER_H
#define BLACK_WORD_DIALOG_HELPER_H

#include <QDialog>

namespace Ui {
class BlackWordDialogHelper;
}

class BlackWordDialogHelper : public QDialog
{
    Q_OBJECT

public:
    explicit BlackWordDialogHelper(QWidget *parent = nullptr);
    ~BlackWordDialogHelper();

    static bool doneBtnPressed;

    static std::string boxString;

private slots:
    void on_cancelBtn_clicked();

    void on_doneBtn_clicked();

    void on_blackWordBox_textChanged(const QString &arg1);

private:
    Ui::BlackWordDialogHelper *ui;
};

#endif // BLACK_WORD_DIALOG_HELPER_H

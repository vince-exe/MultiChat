#ifndef BLACK_WORDS_DIALOG_H
#define BLACK_WORDS_DIALOG_H

#include <QDialog>

namespace Ui {
class BlackWordsDialog;
}

class BlackWordsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BlackWordsDialog(QWidget *parent = nullptr);
    ~BlackWordsDialog();

    static bool updtAddWord;

private slots:
    void on_updtWord_clicked();

    void on_addWord_clicked();

    void on_deleteWord_clicked();

private:
    Ui::BlackWordsDialog *ui;
};

#endif // BLACK_WORDS_DIALOG_H

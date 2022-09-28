#ifndef BLACK_WORDS_DIALOG_H
#define BLACK_WORDS_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "chat_utilities.h"

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

    QStandardItemModel* blackWordsModel;

    QStringList horizontalHeader;
    QStringList verticalHeader;

private slots:
    void on_updtWord_clicked();

    void on_addWord_clicked();

    void on_deleteWord_clicked();

    void printBlackWordsList(std::vector<std::string>* vec);
private:
    Ui::BlackWordsDialog *ui;
};

#endif // BLACK_WORDS_DIALOG_H

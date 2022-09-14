#ifndef MUTE_LIST_H
#define MUTE_LIST_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>

namespace Ui {
class MuteList;
}

class MuteList : public QDialog
{
    Q_OBJECT

public:
    explicit MuteList(QWidget *parent = nullptr);
    ~MuteList();

    /* save the selected user */
    std::string selectedUser;

    /* print the muted list */
    void printMutedList(std::vector<std::string>* vec);

    QStandardItemModel* mutedModel;

    QStringList horizontalHeaderUsers;
    QStringList verticalHeaderUsers;

private slots:
    void on_unMuteBtn_clicked();

    void on_doneBtn_clicked();

    void on_mutedTable_activated(const QModelIndex &index);

private:
    Ui::MuteList *ui;
};

#endif // MUTE_LIST_H

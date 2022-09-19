#ifndef BAN_LIST_DIALOG_H
#define BAN_LIST_DIALOG_H

#include <QDialog>

#include <QStandardItemModel>
#include <boost/asio.hpp>

namespace Ui {
class BanListDialog;
}

class BanListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BanListDialog(QWidget *parent = nullptr);
    ~BanListDialog();

    /* catche the name of the selected user */
    std::string selectedUser;

    /* print the ban list */
    void printBanList(std::map<std::string, std::string>* banMap);

    QStandardItemModel* banModel;

    QStringList horizontalHeaderUsers;
    QStringList verticalHeaderUsers;

private slots:
    void on_banTable_activated(const QModelIndex &index);

    void on_doneBtn_clicked();

    void on_pardonBtn_clicked();

private:
    Ui::BanListDialog *ui;
};

#endif // BAN_LIST_DIALOG_H

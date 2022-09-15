#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

/* forms */
#include "client_side_dialog.h"
#include "server_side_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
QT_END_NAMESPACE

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

private slots:
    void on_createBtn_clicked();

    void on_connectBtn_clicked();

private:
    Ui::MainDialog *ui;

    ClientSideDialog* clientSideDialog;

    ServerSideDialog* serverSideDialog;
};

#endif // MAINDIALOG_H

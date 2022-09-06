#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
QT_END_NAMESPACE

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

    static bool enterAsServer;

private slots:
    void on_createBtn_clicked();

    void on_connectBtn_clicked();

private:
    Ui::MainDialog *ui;
};
#endif // MAINDIALOG_H

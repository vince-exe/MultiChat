#ifndef PATH_HELPER_DIALOG_H
#define PATH_HELPER_DIALOG_H

#include <QDialog>
#include <QFileSystemModel>

#include <windows.h>
#include <lmcons.h>

namespace Ui {
class PathHelperDialog;
}

class PathHelperDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PathHelperDialog(QWidget *parent = nullptr);
    ~PathHelperDialog();

    std::string defaultPathOption;

    static bool pathOpened;

    static std::string selectedPath;

    /* return true if the given path exist */
    static bool IsPathExist(const std::string &s);

private slots:
    void on_resetPathBtn_clicked();

    void on_treeView_clicked(const QModelIndex &index);

    void on_browseBtn_clicked();

    void on_doneBtn_clicked();

private:
    Ui::PathHelperDialog *ui;

    QFileSystemModel* dirFilemodel = new QFileSystemModel(this);;
};

#endif // PATH_HELPER_DIALOG_H

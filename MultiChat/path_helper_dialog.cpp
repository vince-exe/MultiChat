#include "path_helper_dialog.h"
#include "ui_path_helper_dialog.h"

#include <sys/stat.h>
#include <QMessageBox>

bool PathHelperDialog::pathOpened;

std::string PathHelperDialog::selectedPath;

PathHelperDialog::PathHelperDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PathHelperDialog) {
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));

    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    /* get the host name */
    TCHAR username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;

    GetUserName((TCHAR*)username, &username_len);

    QString hostName = QString::fromWCharArray(username);

    this->defaultPathOption = hostName.toStdString();

    dirFilemodel->setRootPath(QString::fromStdString(this->defaultPathOption));
    dirFilemodel->setReadOnly(false);
    dirFilemodel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    dirFilemodel->setNameFilterDisables(false);

    ui->treeView->setModel(dirFilemodel);
    ui->treeView->setRootIndex(dirFilemodel->index(QString::fromStdString(this->defaultPathOption)));
    ui->treeView->setColumnWidth(0, 175);
    ui->treeView->header()->hideSection(1);

    ui->pathBox->setText(QString::fromStdString(this->defaultPathOption));
    PathHelperDialog::pathOpened = false;
}

PathHelperDialog::~PathHelperDialog() {
    delete ui;
}

bool PathHelperDialog::IsPathExist(const std::string &s) {
    struct stat buffer;

    return (stat (s.c_str(), &buffer) == 0);
}

void PathHelperDialog::on_resetPathBtn_clicked() {
    /* set the default path as root path */
    dirFilemodel->setRootPath(QString::fromStdString(this->defaultPathOption));

    ui->treeView->setRootIndex(dirFilemodel->index(QString::fromStdString(this->defaultPathOption)));
    ui->pathBox->setText(QString::fromStdString(this->defaultPathOption));
}

void PathHelperDialog::on_treeView_clicked(const QModelIndex &index) {
    ui->pathBox->setText(dirFilemodel->filePath(index));
}

void PathHelperDialog::on_browseBtn_clicked() {
    if(this->IsPathExist(ui->pathBox->text().toStdString())) {
        dirFilemodel->setRootPath(ui->pathBox->text());
        ui->treeView->setRootIndex(dirFilemodel->index(ui->pathBox->text()));
    }
    else {
        QMessageBox::warning(0, "Warning", "Please enter a correct path");
        ui->pathBox->setText(QString::fromStdString(this->defaultPathOption));
        ui->treeView->setRootIndex(dirFilemodel->index(QString::fromStdString(this->defaultPathOption)));
        return;
    }
}


void PathHelperDialog::on_doneBtn_clicked() {
    PathHelperDialog::selectedPath = ui->pathBox->text().toStdString();
    PathHelperDialog::pathOpened = true;

    this->close();
}


#include "options_server_dialog.h"
#include "ui_options_server_dialog.h"

#include "server_side_dialog.h"

#include <fstream>

/* forms */
#include "path_helper_dialog.h"

#include <QMessageBox>

bool OptionsServerDialog::serverOpened;

bool OptionsServerDialog::wantToCLose;

OptionsServerDialog::OptionsServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsServerDialog) {
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));

    OptionsServerDialog::serverOpened = false;
    OptionsServerDialog::wantToCLose = false;
}

OptionsServerDialog::~OptionsServerDialog() {
    delete ui;
}

void OptionsServerDialog::writeIntoFile(std::ofstream& f, std::vector<std::string> *vec) {
    for(auto& str : *vec) {
        str.append("\n");
        f << str;
    }
}

const std::string OptionsServerDialog::currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%d-%m-%Y.%X", &tstruct);

    return buf;
}

void OptionsServerDialog::on_startServerBtn_clicked() {
    if(ServerSideDialog::isServerOpen) {
        QMessageBox::warning(0, "Warning", "The server is already open");
        OptionsServerDialog::serverOpened = false;
        return;
    }

    QMessageBox::information(0, "Success", "Successfully opened the server");
    OptionsServerDialog::serverOpened = true;
    this->close();
}

void OptionsServerDialog::on_closeServerBtn_clicked() {
    if(!ServerSideDialog::isServerOpen) {
        QMessageBox::warning(0, "Warning", "The server is already closed");
        OptionsServerDialog::serverOpened = false;
        return;
    }

    QMessageBox::information(0, "Success", "Successfully closed the server");
    OptionsServerDialog::wantToCLose = true;
    this->close();
}

void OptionsServerDialog::on_backupChatBtn_clicked() {
    PathHelperDialog pathHelperDialog;
    pathHelperDialog.setModal(true);
    pathHelperDialog.show();
    pathHelperDialog.exec();

    if(PathHelperDialog::pathOpened) {
        qDebug() << QString::fromStdString(PathHelperDialog::selectedPath);
    }
}

/* Create a file at the given path, that contains the list of all the users */
void OptionsServerDialog::on_backupUsersBtn_clicked() {
    /* take the path */
    PathHelperDialog pathHelperDialog;
    pathHelperDialog.setModal(true);
    pathHelperDialog.show();
    pathHelperDialog.exec();

    if(!PathHelperDialog::pathOpened) { return; }

    /* store all the names in a vector */
    std::vector<std::string> tmpVec;
    for(auto& key : ServerSideDialog::server->getClientList()) {
        tmpVec.push_back(key.first);
    }
    QString finalPath = QString::fromStdString(PathHelperDialog::selectedPath).replace("/", "\\");

    QString time = QString::fromStdString(this->currentDateTime());
    time.replace(":", "-");

    if(PathHelperDialog::selectedPath.length() == 3) {
        finalPath.append(QString::fromStdString("BackupUsers" + time.toStdString() + ".txt"));
    }
    else {
        finalPath.append(QString::fromStdString("\\BackupUsers" + time.toStdString() + ".txt"));
    }

    std::ofstream f(finalPath.toStdString());

    if(!f.is_open()) {
        QMessageBox::critical(0, "Error", "The application failed to save the list of users");
        return;
    }

    f << "List of the users that were in the chat the " + this->currentDateTime() << std::endl;
    this->writeIntoFile(f, &tmpVec);
    f.close();

    QMessageBox::information(0, "Success", "Successfully saved the users list");
}

void OptionsServerDialog::on_backupWordsBtn_clicked() {
    PathHelperDialog pathHelperDialog;
    pathHelperDialog.setModal(true);
    pathHelperDialog.show();
    pathHelperDialog.exec();

    if(!PathHelperDialog::pathOpened) { return; }

    QString finalPath = QString::fromStdString(PathHelperDialog::selectedPath).replace("/", "\\");

    QString time = QString::fromStdString(this->currentDateTime());
    time.replace(":", "-");

    if(PathHelperDialog::selectedPath.length() == 3) {
        finalPath.append(QString::fromStdString("BackupBlackWords" + time.toStdString() + ".txt"));
    }
    else {
        finalPath.append(QString::fromStdString("\\BackupBlackWords" + time.toStdString() + ".txt"));
    }

    std::ofstream f(finalPath.toStdString());
    if(!f.is_open()) {
        QMessageBox::critical(0, "Error", "The application failed to save the list of users");
        return;
    }

    f << "List of the black words that were in the black list of the server the " + this->currentDateTime() << std::endl;
    this->writeIntoFile(f, &ServerSideDialog::blackWordsVec);
    f.close();

    QMessageBox::information(0, "Success", "Successfully saved the black words list");
}

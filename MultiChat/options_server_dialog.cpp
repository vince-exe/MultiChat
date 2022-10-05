#include "options_server_dialog.h"
#include "ui_options_server_dialog.h"

#include "server_side_dialog.h"

#include <fstream>

/* forms */
#include "path_helper_dialog.h"

#include <QMessageBox>
#include <QRegularExpression>

bool OptionsServerDialog::serverOpened;

bool OptionsServerDialog::wantToCLose;

QString OptionsServerDialog::chatBox;

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

    if(!PathHelperDialog::pathOpened) { return; }

    QStringList list = this->chatBox.split(QRegularExpression("[\n]"));

    QString finalPath = QString::fromStdString(PathHelperDialog::selectedPath).replace("/", "\\");

    QString time = QString::fromStdString(this->currentDateTime());
    time.replace(":", "-");

    if(PathHelperDialog::selectedPath.length() == 3) {
        finalPath.append(QString::fromStdString("BackupChat" + time.toStdString() + ".txt"));
    }
    else {
        finalPath.append(QString::fromStdString("\\BackupChat" + time.toStdString() + ".txt"));
    }

    std::ofstream f(finalPath.toStdString());

    if(!f.is_open()) {
        QMessageBox::critical(0, "Error", "The application failed to save the chat");
        return;
    }

    f << "Backup chat at the " + this->currentDateTime() << std::endl;
    for(auto& word : list) {
        f << word.toStdString() << std::endl;
    }
    f.close();

    QMessageBox::information(0, "Success", "Successfully saved the chat");
    qDebug() << list;
}

/* Create a file at the given path, that contains the list of all the users */
void OptionsServerDialog::on_backupUsersBtn_clicked() {
    /* take the path */
    PathHelperDialog pathHelperDialog;
    pathHelperDialog.setModal(true);
    pathHelperDialog.show();
    pathHelperDialog.exec();

    if(!PathHelperDialog::pathOpened) { return; }

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

    f << "List of the users that were in the chat the " + this->currentDateTime();
    for(auto& key : ServerSideDialog::server->getClientList()) {
        f << key.first << std::endl;
    }
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
    for(auto& str : ServerSideDialog::blackWordsVec) {
        f << str << std::endl;
    }
    f.close();

    QMessageBox::information(0, "Success", "Successfully saved the black words list");
}

#include "black_word_dialog_helper.h"
#include "ui_black_word_dialog_helper.h"

#include <QMessageBox>

#include "chat_utilities.h"
#include "nickname_dialog.h"
#include "server_side_dialog.h"
#include "black_words_dialog.h"

bool BlackWordDialogHelper::doneBtnPressed;

std::string BlackWordDialogHelper::boxString;

BlackWordDialogHelper::BlackWordDialogHelper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlackWordDialogHelper) {
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));

    if(BlackWordsDialog::uptWord) {
        ui->blackWordBox->setText(QString::fromStdString(BlackWordsDialog::selectedWord));
    }
    BlackWordDialogHelper::doneBtnPressed = false;
}

BlackWordDialogHelper::~BlackWordDialogHelper() {
    delete ui;
}

void BlackWordDialogHelper::on_cancelBtn_clicked() {
    this->close();
}

void BlackWordDialogHelper::on_doneBtn_clicked() {
    BlackWordDialogHelper::boxString = ui->blackWordBox->text().toStdString();

    if(boxString.length() < ChatUtilities::minBlackWordLen) { return; }

    if(NicknameDialog::isInVector(&ServerSideDialog::blackWordsVec, BlackWordDialogHelper::boxString)) {
        QMessageBox::warning(0, "Warning", "This black word already exist");
        return;
    }

    BlackWordDialogHelper::doneBtnPressed = true;
    this->close();
}


void BlackWordDialogHelper::on_blackWordBox_textChanged(const QString &arg1) {
    if(arg1.length() > ChatUtilities::maxBlackWordLen) { ui->blackWordBox->backspace(); }
}

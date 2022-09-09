#include "nickname_dialog.h"
#include "ui_nickname_dialog.h"

#include "chat_utilities.h"

#include <QMessageBox>

std::string NicknameDialog::nickname;

bool NicknameDialog::insertNickname;

NicknameDialog::NicknameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NicknameDialog) {
    ui->setupUi(this);

    /* reduce the borders of the window */
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));
    /* align the items */
    ui->titleLabel->setAlignment(Qt::AlignCenter);
    ui->lineLabel->setAlignment(Qt::AlignCenter);

    NicknameDialog::insertNickname = false;
}

NicknameDialog::~NicknameDialog() {
    delete ui;
}

void NicknameDialog::on_cancelBtn_clicked() {
    NicknameDialog::insertNickname = false;
    this->close();
}

void NicknameDialog::on_doneBtn_clicked() {
    /* check that there is max one with space */
    if(!ChatUtilities::checkStringSpaces(ui->nickBox->text(), " ", 1)) {
        QMessageBox::warning(0, "Warning", "The nickname can contain only a white space");
        return;
    }
    else if(ui->nickBox->text().length() < ChatUtilities::minNickLenght) {
        QMessageBox::warning(0, "Warning", "The nickname must contain at least " + QString::number(ChatUtilities::minNickLenght) + " characters");
        return;
    }

    NicknameDialog::nickname = ui->nickBox->text().toStdString();
    NicknameDialog::insertNickname = true;
    this->close();
}

/* check the given nickname */
void NicknameDialog::on_nickBox_textChanged(const QString &arg1) {
    /* check the max lenght */
    if(arg1.length() > ChatUtilities::maxNickLenght) {
        ui->nickBox->backspace(); return;
    }
}

#include "nickname_dialog.h"
#include "ui_nickname_dialog.h"

#include "chat_utilities.h"
#include "client.h"
#include "ip_port_dialog.h"

#include <QMessageBox>

std::string NicknameDialog::nickname;

bool NicknameDialog::insertNickname;

bool NicknameDialog::checkNickName(std::vector<std::string> *vec, std::string &str) {
    bool exist = false;
    for(auto& s : *vec) {
        if(s == str) {
            exist = true;
        }
    }

    return exist;
}

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
    if(!ChatUtilities::checkStringCharacter(ui->nickBox->text(), " ", 1)) {
        QMessageBox::warning(0, "Warning", "The nickname can contain only a white space");
        return;
    }
    else if(ui->nickBox->text().length() < ChatUtilities::minNickLenght) {
        QMessageBox::warning(0, "Warning", "The nickname must contain at least " + QString::number(ChatUtilities::minNickLenght) + " characters");
        return;
    }

    /* check if the nickname already exist */
    Client client;
    /* connect to the server */
    if(!client.connect(IpPortDialog::ipAddress, IpPortDialog::port, "TEST")) {
        QMessageBox::critical(0, "Error", "The server is not responding...");
        client.close();
        return;
    }
    NicknameDialog::nickname = ui->nickBox->text().toStdString();

    /* send the message to receive the list of users */
    ChatUtilities::send(client.getSocket(), ChatMessages::giveUsersList + ChatMessages::termCharacter);
    /* receive the size of the users list */
    std::string sizeToStd = ChatUtilities::read_until(client.getSocket(), ChatMessages::termCharacter);
    size_t sizeT;

    /* convert the size from std::string to size_t */
    sscanf(sizeToStd.c_str(), "%zu", &sizeT);
    std::vector<std::string> clientNames(sizeT);

    client.getSocket()->read_some( boost::asio::buffer(clientNames, sizeT));
    client.close();

    /* check if the nick already exist */
    if(NicknameDialog::checkNickName(&clientNames, nickname)) {
        QMessageBox::warning(0, "Warning", "Nickname already exist");
        return;
    }
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

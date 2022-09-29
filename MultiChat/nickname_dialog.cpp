#include "nickname_dialog.h"
#include "ui_nickname_dialog.h"

#include "chat_utilities.h"
#include "client.h"
#include "ip_port_dialog.h"

#include <QMessageBox>
#include <QKeyEvent>

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

std::vector<std::string> NicknameDialog::deserializeList(const std::string& str, const char* dC) {
    std::vector<std::string> vec;
    std::string tmp;

    for(int i = 0; i < str.length(); i++) {
        if(str[i] == *dC) {
            vec.push_back(tmp);
            tmp.clear();
            continue;
        }
        tmp.push_back(str[i]);
    }
    return vec;
}

bool NicknameDialog::isInVector(std::vector<std::string>* vec, const std::string& nick) {
    for(auto& str : *vec) {
        if(str == nick) { return true; }
    }
    return false;
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
    else if(!ChatUtilities::checkStringCharacter(ui->nickBox->text(), ChatMessages::serializationChar.c_str(), 0)) {
        QMessageBox::warning(0, "Warning", "Invalid nickname");
        return;
    }
    else if(ui->nickBox->text().length() < ChatUtilities::minNickLenght) {
        QMessageBox::warning(0, "Warning", "The nickname must contain at least " + QString::number(ChatUtilities::minNickLenght) + " characters");
        return;
    }

    /* check if the nickname already exist */
    Client client;
    /* connect to the server */
    if(!client.connect(IpPortDialog::ipAddress, IpPortDialog::port)) {
        QMessageBox::critical(0, "Error", "The server is not responding...");
        client.close();
        return;
    }
    NicknameDialog::nickname = ui->nickBox->text().toStdString();

    /* send the message to get the client nicknames list and the banned list of ip */
    boost::asio::write(*client.getSocket(), boost::asio::buffer(ChatMessages::checkNickBan + ChatMessages::termCharacter));

    std::string nicknameList = ChatUtilities::read_until(client.getSocket(), ChatMessages::termCharacter);
    nicknameList.resize(nicknameList.size() - 1);

    std::string banList = ChatUtilities::read_until(client.getSocket(), ChatMessages::termCharacter);
    banList.resize(banList.size() - 1);

    std::vector<std::string> nicknameListVec = NicknameDialog::deserializeList(nicknameList, ChatMessages::serializationChar.c_str());
    std::vector<std::string> banListVec = NicknameDialog::deserializeList(banList, ChatMessages::serializationChar.c_str());

    /* check if the nick already exist */
    if(NicknameDialog::isInVector(&nicknameListVec, NicknameDialog::nickname)) {
        QMessageBox::warning(0, "Warning", "This nickname already exist");
        return;
    }

    boost::asio::ip::tcp::endpoint remoteEp = client.getSocket()->remote_endpoint();
    boost::asio::ip::address remoteAd = remoteEp.address();
    client.close();

    /* check if client is banned */
    if(NicknameDialog::isInVector(&banListVec, remoteAd.to_string())) {
        QMessageBox::warning(0, "Warning", "You are banned from this server");
        return;
    }

    NicknameDialog::insertNickname = true;
    this->close();
    return;
}

/* check the given nickname */
void NicknameDialog::on_nickBox_textChanged(const QString &arg1) {
    /* check the max lenght */
    if(arg1.length() > ChatUtilities::maxNickLenght) {
        ui->nickBox->backspace(); return;
    }
}

void NicknameDialog::keyPressEvent(QKeyEvent *event) {
    /* check if the user pressed ENTER */
    if((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        this->on_doneBtn_clicked();
    }
}

#include "black_words_dialog.h"
#include "ui_black_words_dialog.h"

#include <QKeyEvent>

#include "server_side_dialog.h"
#include "black_word_dialog_helper.h"
#include "chat_utilities.h"
#include "nickname_dialog.h"

bool BlackWordsDialog::updtAddWord;

bool BlackWordsDialog::uptWord;

std::string BlackWordsDialog::selectedWord;

int BlackWordsDialog::selectedRow;

BlackWordsDialog::BlackWordsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlackWordsDialog) {
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));

    BlackWordsDialog::updtAddWord = false;
    BlackWordsDialog::uptWord = false;
    BlackWordsDialog::selectedWord = "..";

    /* alloc the QStandardItemModels */
    this->blackWordsModel = new QStandardItemModel();
    this->horizontalHeader.append("BlackWords");

    /* set the models */
    this->blackWordsModel->setHorizontalHeaderLabels(this->horizontalHeader);
    this->blackWordsModel->setVerticalHeaderLabels(this->verticalHeader);

    /* table settings */
    ui->blackWordsTable->verticalHeader()->setVisible(false);
    ui->blackWordsTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->blackWordsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->blackWordsTable->verticalHeader()->setDefaultSectionSize(35);

    ui->blackWordsTable->setModel(this->blackWordsModel);
    this->printBlackWordsList(&ServerSideDialog::blackWordsVec);
}

BlackWordsDialog::~BlackWordsDialog() {
    delete ui;
}

/* Update a Black Word */
void BlackWordsDialog::on_updtWord_clicked() {
    if(!NicknameDialog::isInVector(&ServerSideDialog::blackWordsVec, BlackWordsDialog::selectedWord)) { return; }

    BlackWordsDialog::uptWord = true;
    BlackWordDialogHelper blackWordsDialogHelper;
    blackWordsDialogHelper.setModal(true);
    blackWordsDialogHelper.show();
    blackWordsDialogHelper.exec();

    BlackWordsDialog::uptWord = false;
    if(!BlackWordDialogHelper::doneBtnPressed) { return; }

    /* remove the old element */
    ServerSideDialog::blackWordsVec.erase(std::remove(ServerSideDialog::blackWordsVec.begin(), ServerSideDialog::blackWordsVec.end(), BlackWordsDialog::selectedWord));
    /* push the new element */
    ServerSideDialog::blackWordsVec.push_back(BlackWordDialogHelper::boxString);

    ChatUtilities::clearQTableView(ui->blackWordsTable, this->blackWordsModel, ServerSideDialog::blackWordsVec.size() - 1);
    ui->blackWordsTable->setModel(this->blackWordsModel);
    this->printBlackWordsList(&ServerSideDialog::blackWordsVec);

    BlackWordsDialog::updtAddWord = true;
}

/* Add a Black Word */
void BlackWordsDialog::on_addWord_clicked() {
    BlackWordDialogHelper blackWordsDialogHelper;
    blackWordsDialogHelper.setModal(true);
    blackWordsDialogHelper.show();
    blackWordsDialogHelper.exec();

    if(!BlackWordDialogHelper::doneBtnPressed) { return; }

    /* add the black word to the black words list */
    ServerSideDialog::blackWordsVec.push_back(BlackWordDialogHelper::boxString);
    ChatUtilities::clearQTableView(ui->blackWordsTable, this->blackWordsModel, ServerSideDialog::blackWordsVec.size() - 1);
    this->printBlackWordsList(&ServerSideDialog::blackWordsVec);
    BlackWordsDialog::updtAddWord = true;
}

/* Delete a Black Word */
void BlackWordsDialog::on_deleteWord_clicked() {
    if(!NicknameDialog::isInVector(&ServerSideDialog::blackWordsVec, BlackWordsDialog::selectedWord)) { return; }

    ServerSideDialog::blackWordsVec.erase(std::remove(ServerSideDialog::blackWordsVec.begin(), ServerSideDialog::blackWordsVec.end(), BlackWordsDialog::selectedWord));
    ChatUtilities::clearQTableView(ui->blackWordsTable, this->blackWordsModel, ServerSideDialog::blackWordsVec.size() + 1);

    ui->blackWordsTable->setModel(this->blackWordsModel);
    this->printBlackWordsList(&ServerSideDialog::blackWordsVec);

    BlackWordsDialog::updtAddWord = true;
}

void BlackWordsDialog::printBlackWordsList(std::vector<std::string> *vec) {
    int i = 0;
    for(auto&it : *vec) {
        this->blackWordsModel->setItem(i, ChatUtilities::getItem(QString::fromStdString(it)));
        i++;
    }
    this->ui->blackWordsTable->setModel(this->blackWordsModel);
}

void BlackWordsDialog::on_blackWordsTable_activated(const QModelIndex &index) {
    this->selectedWord = index.sibling(index.row(), 0).data().toString().toStdString();
    this->selectedRow = index.row();
}

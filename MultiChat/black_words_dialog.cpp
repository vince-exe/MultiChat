#include "black_words_dialog.h"
#include "ui_black_words_dialog.h"

#include "server_side_dialog.h"
#include "black_word_dialog_helper.h"
#include "chat_utilities.h"

bool BlackWordsDialog::updtAddWord;

BlackWordsDialog::BlackWordsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlackWordsDialog) {
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    /* set the window icon */
    this->setWindowIcon(QIcon("images/mainIcon.png"));

    BlackWordsDialog::updtAddWord = false;

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

}

void BlackWordsDialog::printBlackWordsList(std::vector<std::string> *vec) {
    int i = 0;
    for(auto&it : *vec) {
        this->blackWordsModel->setItem(i, ChatUtilities::getItem(QString::fromStdString(it)));
        i++;
    }
    this->ui->blackWordsTable->setModel(this->blackWordsModel);
}

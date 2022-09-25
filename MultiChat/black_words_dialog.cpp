#include "black_words_dialog.h"
#include "ui_black_words_dialog.h"

#include "server_side_dialog.h"
#include "black_word_dialog_helper.h"

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
    ui->blackWordsTable->verticalHeader()->setDefaultSectionSize(25);

    ui->blackWordsTable->setModel(this->blackWordsModel);
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

    qDebug() << QString::fromStdString(BlackWordDialogHelper::boxString);
    BlackWordsDialog::updtAddWord = true;
}

/* Delete a Black Word */
void BlackWordsDialog::on_deleteWord_clicked() {

}

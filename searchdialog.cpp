#include "searchdialog.h"

#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QWidget* parent) : QDialog(parent), ui(new Ui::SearchDialog) {
    ui->setupUi(this);
    initMainButtons();
}

SearchDialog::~SearchDialog() {
    delete ui;
}

void SearchDialog::initMainButtons() {
    connect(ui->pushButtonFindNext, &QPushButton::clicked, this, &SearchDialog::onButtonFindNext);
    connect(ui->pushButtonReplace, &QPushButton::clicked, this, &SearchDialog::onButtonReplace);
    connect(ui->pushButtonReplaceAll, &QPushButton::clicked, this,
            &SearchDialog::onButtonReplaceAll);
}

void SearchDialog::onButtonFindNext() const {
    emit findNext(ui->plainTextEditSearch->toPlainText());
}

void SearchDialog::onButtonReplace() const {
    emit replace(ui->plainTextEditSearch->toPlainText(), ui->plainTextEditReplace->toPlainText());
}

void SearchDialog::onButtonReplaceAll() const {
    emit replaceAll(ui->plainTextEditSearch->toPlainText(),
                    ui->plainTextEditReplace->toPlainText());
}

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
//    setFixedSize(300, 100);
    ui->lineEdit->setText("127.0.0.1");
}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::getText()
{
    return ui->lineEdit->text();
}

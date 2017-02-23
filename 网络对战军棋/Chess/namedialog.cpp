#include "namedialog.h"
#include "ui_namedialog.h"

NameDialog::NameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NameDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setText("Username");
}

NameDialog::~NameDialog()
{
    delete ui;
}

QString NameDialog::getText()
{
    return ui->lineEdit->text();
}

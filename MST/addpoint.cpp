#include "addpoint.h"
#include "ui_addpoint.h"

AddPoint::AddPoint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPoint)
{
    ui->setupUi(this);
}

AddPoint::~AddPoint()
{
    delete ui;
}

int AddPoint::get_x()
{
    return ui->lineEdit->text().toInt();
}

int AddPoint::get_y()
{
    return ui->lineEdit_2->text().toInt();
}

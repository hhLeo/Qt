#include "clientdialog.h"
#include "ui_clientdialog.h"
#include <QSignalMapper>

ClientDialog::ClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setText("127.0.0.1");

    QSignalMapper *m = new QSignalMapper(this);

    connect(ui->pushButton_0, SIGNAL(clicked()), m, SLOT(map()));
    m->setMapping(ui->pushButton_0, 0);
    connect(ui->pushButton_1, SIGNAL(clicked()), m, SLOT(map()));
    m->setMapping(ui->pushButton_1, 1);
    connect(ui->pushButton_2, SIGNAL(clicked()), m, SLOT(map()));
    m->setMapping(ui->pushButton_2, 2);
    connect(ui->pushButton_3, SIGNAL(clicked()), m, SLOT(map()));
    m->setMapping(ui->pushButton_3, 3);
    connect(ui->pushButton_4, SIGNAL(clicked()), m, SLOT(map()));
    m->setMapping(ui->pushButton_4, 4);
    connect(ui->pushButton_5, SIGNAL(clicked()), m, SLOT(map()));
    m->setMapping(ui->pushButton_5, 5);
    connect(ui->pushButton_6, SIGNAL(clicked()), m, SLOT(map()));
    m->setMapping(ui->pushButton_6, 6);
    connect(ui->pushButton_7, SIGNAL(clicked()), m, SLOT(map()));
    m->setMapping(ui->pushButton_7, 7);
    connect(ui->pushButton_8, SIGNAL(clicked()), m, SLOT(map()));
    m->setMapping(ui->pushButton_8, 8);
    connect(ui->pushButton_9, SIGNAL(clicked()), m, SLOT(map()));
    m->setMapping(ui->pushButton_9, 9);
    connect(ui->pushButton_dot, SIGNAL(clicked()), m, SLOT(map()));
    m->setMapping(ui->pushButton_dot, 10);
    connect(ui->pushButton_del, SIGNAL(clicked()), m, SLOT(map()));
    m->setMapping(ui->pushButton_del, 11);

    connect(m, SIGNAL(mapped(int)), this, SLOT(buttonPressed(int)));
}

ClientDialog::~ClientDialog()
{
    delete ui;
}

QString ClientDialog::getText()
{
    return ui->lineEdit->text();
}

void ClientDialog::buttonPressed(int k)
{
    if (k < 10)
        ui->lineEdit->setText(ui->lineEdit->text() + QString::number(k));
    else if (k == 10)
        ui->lineEdit->setText(ui->lineEdit->text() + QString("."));
    else
    {
        //delete
        QString a = ui->lineEdit->text();
        if (a.size())
        {
            a.remove(a.size() - 1, 1);
            ui->lineEdit->setText(a);
        }
    }
}

#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>

namespace Ui {
class ClientDialog;
}

class ClientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientDialog(QWidget *parent = 0);
    ~ClientDialog();
    QString getText();

private:
    Ui::ClientDialog *ui;

private slots:
    void buttonPressed(int k);
};

#endif // CLIENTDIALOG_H

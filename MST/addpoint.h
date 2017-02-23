#ifndef ADDPOINT_H
#define ADDPOINT_H

#include <QDialog>

namespace Ui {
class AddPoint;
}

class AddPoint : public QDialog
{
    Q_OBJECT

public:
    explicit AddPoint(QWidget *parent = 0);
    ~AddPoint();
    int get_x();
    int get_y();

private:
    Ui::AddPoint *ui;
};

#endif // ADDPOINT_H

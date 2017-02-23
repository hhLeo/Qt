#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "addpoint.h"
#include "chip.h"
#include "delaunay.h"
#include "mode1.h"
#include "dots.h"
#include "view.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QMouseEvent>
#include <QDebug>
#include <vector>
#include <QTextStream>
#include <QString>
#include <QIODevice>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paint(QWidget *parent = 0);

private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_action_Open_triggered();
    void on_action_AddPoint_triggered();
    void on_action_Exit_triggered();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_action_MST_triggered();
    void on_action_Delaunay_triggered();
    void move();
    void dclickAdd(int x, int y);
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    int Mode;
    Ui::MainWindow *ui;
    vector <Dots::Coordinate> dots;
    AddPoint *addPoint;
    vector <Chip*> item;
    QGraphicsScene *scene;
    Delaunay *delaunay;
    vector <Delaunay::Line> ll;
    Mode1 *mode1;
    View *view;
    QString fileName;
    int sum;
    int add_x, add_y;
    vector<QGraphicsLineItem*> lines;
    QGraphicsLineItem* line;
};

#endif // MAINWINDOW_H

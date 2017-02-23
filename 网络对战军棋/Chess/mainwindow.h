#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog.h"
#include "clientdialog.h"
#include "namedialog.h"
#include <QMainWindow>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QFileDialog>
#include <QString>
#include <QThread>
#include <windows.h>
#include <iostream>
#define  PORT 8080


class Pieces
{
public:
    int rank;
    int x;
    int y;
    bool alive;
};

struct ShuJu
{
//    QRectF myRect[5][6], yourRect[5][6];
    Pieces my[5][6], your[5][6];
    bool dir;//0:对方;1:我方
};

class Thread : public QThread
{
    Q_OBJECT
private:
//    int number;
protected:
public:
    Thread(QObject *parent=0){temp = NULL;}
    ~Thread(){}
    void run();
    ShuJu *temp;
signals:
    void tempChanged();
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *e);
    void init();
    void getXY();//给realX, realY赋值
    void rank_();//对应的级别编号
    int yourRank, myRank;
    int result_0();//(your[][].rank-r, my[][].rank-r)when meet:    0:lose   1:equal    2:win
    void result(int res);
    void win();
    void lose();
    void swap(int chooseX, int chooseY);
    bool canNotMove();
    void printData();

private slots:
    void on_startButton_clicked();
    void on_buildButton_clicked();
    void on_connectButton_clicked();
    void on_head1_clicked();

public slots:
    void readDraw();

private:
    Ui::MainWindow *ui;
    QRectF myRect[5][6], yourRect[5][6];
    int w, h;//width,height
    int a[5] = {89, 170, 251, 332, 413};
    int b[12] = {78, 118, 158, 199, 239, 280, 381, 421, 461, 501, 542, 582};
    QString r[12] = {"工兵", "排长", "连长", "营长", "团长", "旅长", "师长", "军长", "司令", "地雷", "军棋", "炸弹"};//rank
    Pieces my[5][6], your[5][6];
    int orig[5][6] = {{9,2,5,11,8,2},{10,1,12,0,12,3},{9,3,6,12,0,5},{4,6,12,2,12,1},{9,6,1,7,11,4}};//origRank

    bool dir;//0:对方;1:我方
    int tempX, tempY;//鼠标点的地方的位置
    int chooseX, chooseY;//Pieces里的x,y
    int real0_x, real0_y, real1_x, real1_y;//实际上的棋子编号
    int choose;//选中的个数
    bool have[5][12];
    bool haveStart;
    Thread *thread;
    QString fileName;
};

#endif // MAINWINDOW_H

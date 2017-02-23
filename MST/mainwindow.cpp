#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qmath.h>
#include "string.h"
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(0, 0, 300, 300, this);

    view = ui->graphicsView;
    view->setScene(scene);
    Mode = 1;
    /*
    scene = new QGraphicsScene(parent);
    view = new View;
    view->setScene(scene);
    view->show();
*/

    connect(view, SIGNAL(mmove()), this, SLOT(move()));
    connect(view, SIGNAL(dclick(int, int)),
            this, SLOT(dclickAdd(int, int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paint(QWidget *parent)
{
    qDebug("paint");
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    vector <Chip*>::iterator it;
    for(it = item.begin(); it != item.end(); it++)
    {
        delete *it;
    }
    item.clear();
    Chip *aa;
    for(int i = 0; i < sum; i++)
    {
        aa = new Chip(dots[i].x,dots[i].y);
        //scene->addElinesipse(dots[i].x,dots[i].y,0.3,0.3);
        item.push_back(aa);
        scene->addItem(aa);
    }

}

void MainWindow::on_action_Open_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Alinesfile(*.*)"));
    //if(fileName != NUlines){
    QFile f(fileName);

    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qFatal("Could not open file");
    }
    QTextStream in(&f);
    in >> sum;
    qDebug()<<sum;
    int i, j;
    Dots::Coordinate read[sum];
    for(int k = 0; k < sum; k++)
    {
        in >> i >> j;
        read[k].x = i;
        read[k].y = j;
    }
    dots.clear();
    for(int k = 0; k < sum; k++)
    {
        dots.push_back(read[k]);
    }
    Delaunay *dd = new Delaunay();
    dd->run(dots);
    paint();
//
    for (int k = 0; k < sum; k++)
        qDebug("%d %d\n", dots[k].x, dots[k].y);
//
    f.close();//}
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    /*
    qreal scale = qPow(qreal(2), (value - 5) / qreal(0.5));

    QMatrix matrix;
    matrix.scale(scale, scale);

    ui->graphicsView->setMatrix(matrix);
    */
    QMatrix matrix;
    matrix.scale(1, 1);

    ui->graphicsView->setMatrix(matrix);
    ui->graphicsView->scale(value/10.0 ,value/10.0);
}

void MainWindow::on_action_AddPoint_triggered()
{
    addPoint = new AddPoint;
    if(addPoint->exec())
    {
        add_x = addPoint->get_x();
        add_y = addPoint->get_y();
        //...add a point to dots
        Dots::Coordinate aa;
        aa.x = add_x;
        aa.y = add_y;
        dots.push_back(aa);
        sum++;
        paint();

        qDebug("%d %d", add_x, add_y);
    }

    if(Mode == 1) on_action_Delaunay_triggered();
    else on_action_MST_triggered();
}

void MainWindow::on_action_Exit_triggered()
{
    qApp->quit();
}

void MainWindow::on_pushButton_clicked()
{
    ui->graphicsView->scale(0.8,0.8);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->graphicsView->scale(1.2,1.2);
}

void MainWindow::on_action_MST_triggered()
{
    Mode = 0;
//    for(int i = 0; i < lines.size(); i++)
//        scene->removeItem(lines[i]);
    vector <QGraphicsLineItem*>::iterator it;
    for(it = lines.begin(); it != lines.end(); it++)
        delete *it;
    lines.clear();

    delaunay = new Delaunay;
    delaunay->run(dots);
    ll = delaunay->getTreeLine();
    for(int i = 0; i < ll.size(); i++)
    {
        line = new QGraphicsLineItem(dots[ll[i].x-1].x, dots[ll[i].x-1].y, dots[ll[i].y-1].x, dots[ll[i].y-1].y);
        lines.push_back(line);
        scene->addItem(line);
    }
    delete delaunay;
}

void MainWindow::on_action_Delaunay_triggered()
{
    Mode = 1;
//    for(int i = 0; i < lines.size(); i++)
//        scene->removeItem(lines[i]);
    vector <QGraphicsLineItem*>::iterator it;
    for(it = lines.begin(); it != lines.end(); it++)
        delete *it;
    lines.clear();

    delaunay = new Delaunay;
    delaunay->run(dots);

    ll = delaunay->getDelaunay();
    for(int i = 0; i < ll.size(); i++)
    {
        line = new QGraphicsLineItem(dots[ll[i].x-1].x, dots[ll[i].x-1].y, dots[ll[i].y-1].x, dots[ll[i].y-1].y);
        lines.push_back(line);
        scene->addItem(line);
    }
    delete delaunay;
}


void MainWindow::move()
{
    qDebug()<<item.size();
    qDebug()<<"hellop"<< dots.size();
    dots.clear();
    qDebug()<<"c;ear";
    for(int i = 0; i < item.size(); i++)
    {
        Dots::Coordinate c;
        c.x = item[i]->pos().x();
        c.y = item[i]->pos().y();
        qDebug()<<i;
        dots.push_back(c);
    }
    if(Mode == 1) on_action_Delaunay_triggered();
    else on_action_MST_triggered();
//    paint();
}

void MainWindow::dclickAdd(int x, int y)
{
    addPoint = new AddPoint;
    qDebug("addPoint");
    Dots::Coordinate aa;
//    aa.x = event->pos().x();
//    aa.y = event->pos().y();
    aa.x = x;
    aa.y = y;
    dots.push_back(aa);
    sum++;
    paint();

    qDebug("%d %d", aa.x, aa.y);

    if(Mode == 1) on_action_Delaunay_triggered();
    else on_action_MST_triggered();
}

void MainWindow::on_pushButton_3_clicked()
{
    mode1 = new Mode1;
    mode1->run(dots);

    std::ostringstream s;
    double t = mode1->use_time;
    s << t;

    ui->label->setText((s.str()+"\tms").c_str());
    delete mode1;
}

void MainWindow::on_pushButton_4_clicked()
{
    delaunay = new Delaunay;
    delaunay->run(dots);

    std::ostringstream s;
    double t = delaunay->use_time;
    s << t;

    ui->label_2->setText((s.str()+"\tms").c_str());
    delete delaunay;
}

#ifndef VIEW_H
#define VIEW_H
#include <QGraphicsView>
#include <QSlider>
#include <QDebug>
#include <QMouseEvent>

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View(QWidget*parent);
    View(){}
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:

signals:
    void mmove();
    void dclick(int, int);
};

#endif // VIEW_H

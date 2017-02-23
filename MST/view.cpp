#include "view.h"

View::View(QWidget *parent):
    QGraphicsView(parent)
{
}

void View::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit dclick(event->pos().x(),event->pos().y());
}

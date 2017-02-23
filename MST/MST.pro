#-------------------------------------------------
#
# Project created by QtCreator 2014-09-04T20:59:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MST
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addpoint.cpp \
    chip.cpp \
    view.cpp

HEADERS  += mainwindow.h \
    addpoint.h \
    delaunay.h \
    dots.h \
    chip.h \
    view.h \
    mode1.h

FORMS    += mainwindow.ui \
    addpoint.ui

RESOURCES += \
    images.qrc

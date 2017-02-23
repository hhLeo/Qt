#-------------------------------------------------
#
# Project created by QtCreator 2014-09-11T16:36:54
#
#-------------------------------------------------
LIBS += -lwsock32
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chess
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    namedialog.cpp \
    clientdialog.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    namedialog.h \
    clientdialog.h

FORMS    += \
    mainwindow.ui \
    dialog.ui \
    namedialog.ui \
    clientdialog.ui

RESOURCES += \
    images.qrc

#-------------------------------------------------
#
# Project created by QtCreator 2016-11-24T16:17:59
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DanmakuClientOld
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    danmu.cpp \
    titlewidget.cpp

HEADERS  += mainwindow.h \
    danmu.h \
    titlewidget.h

FORMS    += mainwindow.ui

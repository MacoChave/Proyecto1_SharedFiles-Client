#-------------------------------------------------
#
# Project created by QtCreator 2018-03-15T20:41:24
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Proyecto1_SharedFiles_Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp

HEADERS  += mainwindow.h \
    login.h \
    List/list.h \
    List/node.h

FORMS    += mainwindow.ui \
    login.ui

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
    GenericTree/nodegenerictree.cpp \
    GenericTree/tadgenerictree.cpp \
    dialoglogin.cpp \
    dialogdocument.cpp

HEADERS  += mainwindow.h \
    List/list.h \
    List/node.h \
    GenericTree/generictree.h \
    GenericTree/nodegenerictree.h \
    GenericTree/tadgenerictree.h \
    dialoglogin.h \
    dialogdocument.h

FORMS    += mainwindow.ui \
    dialoglogin.ui \
    dialogdocument.ui

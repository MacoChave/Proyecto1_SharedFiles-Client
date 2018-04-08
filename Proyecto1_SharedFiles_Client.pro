#-------------------------------------------------
#
# Project created by QtCreator 2018-03-15T20:41:24
#
#-------------------------------------------------

QT       += core gui network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Proyecto1_SharedFiles_Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    GenericTree/nodegenerictree.cpp \
    GenericTree/tadgenerictree.cpp \
    dialoglogin.cpp \
    dialogdocument.cpp \
    dialogpresentacion.cpp \
    DoubleList/tadlist.cpp \
    dialoglienzo.cpp \
    IrregularMatrix/arreglo.cpp

HEADERS  += mainwindow.h \
    List/list.h \
    List/node.h \
    GenericTree/generictree.h \
    GenericTree/nodegenerictree.h \
    GenericTree/tadgenerictree.h \
    dialoglogin.h \
    dialogdocument.h \
    dialogpresentacion.h \
    DoubleList/tadlist.h \
    dialoglienzo.h \
    IrregularMatrix/arreglo.h

FORMS    += mainwindow.ui \
    dialoglogin.ui \
    dialogdocument.ui \
    dialogpresentacion.ui \
    dialoglienzo.ui

RESOURCES +=

DISTFILES += \
    .gitignore

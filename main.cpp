//#include "mainwindow.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.setWindowTitle("Cliente");
//    w.show();

    LogIn v;
    v.setWindowTitle("LogIn");
    v.show();

    return a.exec();
}

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString buttonStyle;
    buttonStyle = "QPushButton {"
                  "border-style: solid;"
                  "border-width: 1px;"
                  "border-color: rgb(255, 255, 255, 255);"
                  "background-color: rgb(255, 255, 255, 180);"
                  "color: #1D1D1D;"
                  "border-radius: 6px;"
                  "}";

    QString frameStyle;
    frameStyle = "QFrame {"
                 "border-style: solid;"
                 "border-width: 1px;"
                 "border-color: rgb(255, 255, 255, 180);"
                 "background-color: rgb(255, 255, 255, 100);"
                 "}"
                 "QLabel {"
                 "border-style: solid;"
                 "border-width: 1px;"
                 "border-color: rgb(255, 255, 255, 100);"
                 "background-color: rgb(255, 255, 255, 50);"
                 "}";

    QString editTextStyle;
    editTextStyle = "QLineEdit {"
                    "background-color: rgb(245, 245, 245);"
                    "color: rgb(29, 29, 29);"
                    "}"
                    "QTextEdit {"
                    "background-color: rgb(245, 245, 245);"
                    "color: rgb(29, 29, 29);"
                    "}";

    QString windowStyle;
    windowStyle = "QMainWindow {"
                  "background-color: "
                  "qlineargradient(spread:pad, "
                  "x1:0.5, y1:1, x2:0.5, y2:0, "
                  "stop:0.0700935 rgba(47, 41, 61, 255), "
                  "stop:0.929907 rgba(0, 180, 156, 255));"
                  "color: rgb(255, 255, 255);"
                  "}";

    QString dialogStyle;
    dialogStyle = "QDialog {"
                  "background-color: "
                  "qlineargradient(spread:pad, "
                  "x1:0.5, y1:1, x2:0.5, y2:0, "
                  "stop:0.0700935 rgba(47, 41, 61, 255), "
                  "stop:0.929907 rgba(0, 180, 156, 255));"
                  "color: rgb(255, 255, 255);"
                  "}";

    qApp->setStyleSheet(buttonStyle + frameStyle + windowStyle + dialogStyle);

    MainWindow w;
    w.setWindowTitle("Perfil de usuario");
    w.show();

    return a.exec();
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnMensaje_clicked();

public slots:
    void consumer();

    void producer(QString value);

private:
    Ui::MainWindow *ui;

    QTcpSocket *tcpCliente;
    void interpretarMensaje(QString mensaje);
};

#endif // MAINWINDOW_H

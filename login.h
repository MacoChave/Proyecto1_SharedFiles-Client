#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QStringList>
#include <QString>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class LogIn;
}

class LogIn : public QWidget
{
    Q_OBJECT

public:
    explicit LogIn(QWidget *parent = 0);
    ~LogIn();

private slots:
    void on_btnLogIn_clicked();

    void on_btnLogUp_clicked();

    void leerServidor();

private:
    Ui::LogIn *ui;

    QTcpSocket *tcpCliente;
    void interpretarMensaje(QString mensaje);
};

#endif // LOGIN_H

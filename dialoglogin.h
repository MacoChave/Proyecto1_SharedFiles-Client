#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QStringList>
#include <QString>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class DialogLogin;
}

class DialogLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLogin(QWidget *parent = 0);
    ~DialogLogin();

    void setTcp(QTcpSocket *value);

private slots:
    void on_btnLogIn_clicked();

    void on_btnLogUp_clicked();

    void consumer();

    void producer(QString value);

private:
    Ui::DialogLogin *ui;
    QTcpSocket *tcpCliente;

    void interpreter(QString mensaje);
    void conectar();
};

#endif // DIALOGLOGIN_H

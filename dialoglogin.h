#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QStringList>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QList>
#include <QPushButton>

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

    QList<QPushButton *> botones;

    void actionLogIn(QStringList value);
    void actionLogUp(QStringList value);

    void conectar();
    void interpreter(QString mensaje);
};

#endif // DIALOGLOGIN_H

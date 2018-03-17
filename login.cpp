#include "login.h"
#include "ui_login.h"

LogIn::LogIn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogIn)
{
    ui->setupUi(this);

    tcpCliente = new QTcpSocket(this);
    tcpCliente->connectToHost(QHostAddress::LocalHost, 1234);
    connect(
                tcpCliente,
                SIGNAL (readyRead()),
                this,
                SLOT (leerServidor())
            );
}

LogIn::~LogIn()
{
    delete ui;
}

void LogIn::on_btnLogIn_clicked()
{
    QString mensaje("<LOGIN><");
    mensaje.append(ui->edtNick->text());
    mensaje.append("><");
    mensaje.append(ui->edtPass->text());
    mensaje.append(">");

    tcpCliente->write(
                        mensaje.toLatin1().data(),
                        mensaje.size()
                    );
}

void LogIn::on_btnLogUp_clicked()
{

}

void LogIn::leerServidor()
{
    QByteArray buffer;
    buffer.resize(tcpCliente->bytesAvailable());
    tcpCliente->read(buffer.data(), buffer.size());
    interpretarMensaje(QString (buffer));
}

void LogIn::interpretarMensaje(QString mensaje)
{
    qDebug() << mensaje;
    QStringList splMensaje = mensaje.split("^");
    qDebug() << splMensaje;
}

#include "dialoglogin.h"
#include "ui_dialoglogin.h"

DialogLogin::DialogLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLogin)
{
    ui->setupUi(this);
    conectar();
}

DialogLogin::~DialogLogin()
{
    delete ui;
}

/***********************************************************************************
 * MANEJO DE CONEXION CLIENTE
 **********************************************************************************/
void DialogLogin::conectar()
{
    tcpCliente = new QTcpSocket(this);
    tcpCliente->connectToHost(QHostAddress::LocalHost, 1234);
    connect(
                tcpCliente,
                SIGNAL (readyRead()),
                this,
                SLOT (consumer())
                );
}

void DialogLogin::consumer()
{
    QByteArray buffer;
    buffer.resize(tcpCliente->bytesAvailable());
    tcpCliente->read(buffer.data(), buffer.size());
    interpreter(QString (buffer));
}

void DialogLogin::producer(QString value)
{
    tcpCliente->write(
                        value.toLatin1().data(),
                        value.size()
                );
}

void DialogLogin::interpreter(QString mensaje)
{
    QStringList splMensaje = mensaje.split("^");

    if (mensaje.startsWith("LOGIN"))
        actionLogIn(splMensaje);
    else if (mensaje.startsWith("LOGUP"))
        actionLogUp(splMensaje);
}

void DialogLogin::actionLogIn(QStringList value)
{
    if (value[1].compare("CORRECTO") == 0)
    {
        QMessageBox msg;
        msg.setWindowTitle("Información");
        msg.setText("Credenciales correctas");
        msg.exec();

        accept();
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle("Información");
        msg.setText("Credenciales incorrectas");
        msg.exec();

        return;
    }
}

void DialogLogin::actionLogUp(QStringList value)
{
    if (value[1].compare("CORRECTO") == 0)
    {
        QMessageBox msg;
        msg.setWindowTitle("Información");
        msg.setText("Usuario creado satisfactoriamente");
        msg.exec();

        accept();
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle("Información");
        msg.setText("Usuario no creado");
        msg.exec();

        return;
    }
}

/***********************************************************************************
  MANEJO DE METODOS DE CLIENTE
 **********************************************************************************/
void DialogLogin::on_btnLogIn_clicked()
{
    QString user = ui->edt_Login_Nickname->text();
    QString pass = ui->edt_Login_Pass->text();

    if (user.isEmpty() || pass.isEmpty())
    {
        QMessageBox msg;
        msg.setWindowTitle("Precaución");
        msg.setText("No se permiten campos vacíos");
        msg.exec();

        return;
    }

    QString mensaje("LOGIN^");
    mensaje.append(user);
    mensaje.append("^");
    mensaje.append(pass);

    producer(mensaje);
}

void DialogLogin::on_btnLogUp_clicked()
{
    QString user = ui->edt_Logup_Nickname->text();
    QString nombre = ui->edt_Logup_Nombre->text();
    QString correo = ui->edt_Logup_Correo->text();
    QString pass = ui->edt_Logup_Pass->text();

    QString mensaje("LOGUP^");
    mensaje.append(user);
    mensaje.append("^");
    mensaje.append(pass);
    mensaje.append("^");
    mensaje.append(nombre);
    mensaje.append("^");
    mensaje.append(correo);

    producer(mensaje);
}

void DialogLogin::setTcp(QTcpSocket *value)
{
    tcpCliente = value;
}

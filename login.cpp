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
                SLOT (consumer())
            );
}

LogIn::~LogIn()
{
    delete ui;
}

void LogIn::on_btnLogIn_clicked()
{
    QString user = ui->edt_Login_Nickname->text();
    QString pass = ui->edt_Login_Pass->text();

    if (user.isEmpty() || pass.isEmpty())
    {
        QMessageBox msg(this);
        msg.setWindowTitle("Warning");
        msg.setText("No se admiten campos vacíos");
        msg.exec();
        return;
    }

    QString mensaje("LOGIN^");
    mensaje.append(user);
    mensaje.append("^");
    mensaje.append(pass);

    producer(mensaje);
}

void LogIn::on_btnLogUp_clicked()
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

void LogIn::consumer()
{
    QByteArray buffer;
    buffer.resize(tcpCliente->bytesAvailable());
    tcpCliente->read(buffer.data(), buffer.size());
    interpretarMensaje(QString (buffer));
}

void LogIn::producer(QString value)
{
    tcpCliente->write(
                        value.toLatin1().data(),
                        value.size()
                    );
}

void LogIn::interpretarMensaje(QString mensaje)
{
    QStringList splMensaje = mensaje.split("^");

    if (mensaje.startsWith("LOGIN"))
    {
        if (splMensaje[1].compare("CORRECTO") == 0)
        {
            QMessageBox msg(this);
            msg.setWindowTitle("Informacion");
            msg.setText("Credenciales aprobadas");
            msg.exec();

            w.setWindowTitle("Cliente");
            w.show();

            delete tcpCliente;

            this->hide();

            return;
        }
        else
        {
            QMessageBox msg(this);
            msg.setWindowTitle("Informacion");
            msg.setText("Credenciales incorrectas");
            msg.exec();
            return;
        }
    }
    else if (mensaje.startsWith("LOGUP"))
    {
        if (splMensaje[1].compare("CORRECTO") == 0)
        {
            QMessageBox msg(this);
            msg.setWindowTitle("Informacion");
            msg.setText("Usuario creado satisfactoriamente");
            msg.exec();

            return;
        }
        else
        {
            QMessageBox msg(this);
            msg.setWindowTitle("Informacion");
            msg.setText("Usuario no creado");
            msg.exec();
            return;
        }
    }
}

#include "dialoglogin.h"
#include "ui_dialoglogin.h"

DialogLogin::DialogLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLogin)
{
    ui->setupUi(this);
}

DialogLogin::~DialogLogin()
{
    delete ui;
}

void DialogLogin::setTcpClient(QTcpSocket *value)
{
    tcpCliente = value;
}

void DialogLogin::on_btnLogIn_clicked()
{
    QString user = ui->edt_Login_Nickname->text();
    QString pass = ui->edt_Login_Pass->text();

    if (user.isEmpty() || pass.isEmpty())
    {
        QMessageBox::warning(
                    this,
                    "Precaución",
                    "No se admiten campos vacíos"
                    );
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
    {
        if (splMensaje[1].compare("CORRECTO") == 0)
        {
            QMessageBox::information(
                        this,
                        "Informacion",
                        "Credenciales aprobadas"
                        );

            accept();
            this->close();
        }
        else
        {
            QMessageBox::information(
                        this,
                        "Informacion",
                        "Credenciales no aprobadas"
                        );
            return;
        }
    }
    else if (mensaje.startsWith("LOGUP"))
    {
        if (splMensaje[1].compare("CORRECTO") == 0)
        {
            QMessageBox::information(
                        this,
                        "Informacion",
                        "Usuario creado satisfactoriamente"
                        );
            accept();
        }
        else
        {
            QMessageBox::information(
                        this,
                        "Informacion",
                        "Usuario no creado"
                        );
            return;
        }
    }
}

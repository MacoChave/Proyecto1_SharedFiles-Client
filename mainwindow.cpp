#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    usuario = "";

    conectar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::conectar()
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

void MainWindow::consumer()
{
    QByteArray buffer;
    buffer.resize(tcpCliente->bytesAvailable());
    tcpCliente->read(buffer.data(), buffer.size());
    interpretarMensaje(QString (buffer));
}

void MainWindow::producer(QString value)
{
    tcpCliente->write(
                        value.toLatin1().data(),
                        value.size()
                    );
}

void MainWindow::interpretarMensaje(QString mensaje)
{
    QStringList splMensaje = mensaje.split("^");
    qDebug() << splMensaje;

    if (mensaje.startsWith("SESSION"))
    {
        if (splMensaje.size() > 1)
        {
            usuario = splMensaje[1];
            ui->lblUsuario->setText(usuario);
        }
    }
}

void MainWindow::on_btnLogin_clicked()
{
    DialogLogin l(this);
    l.setWindowTitle("LogIn / LogUp");
    l.setTcpClient(tcpCliente);
    l.exec();

    producer("SESSION^USUARIO");
}

void MainWindow::on_btnLogout_clicked()
{
    producer("LOGOUT^SESSION");
    ui->lblUsuario->clear();
    usuario.clear();
}

void MainWindow::on_btnActualizar_clicked()
{
    producer("SESSION^USUARIO");
}

void MainWindow::on_btnEliminar_clicked()
{

}

void MainWindow::on_btnVer_clicked()
{

}

void MainWindow::on_btnNuevo_clicked()
{

}

void MainWindow::on_btnLienzo_clicked()
{

}

void MainWindow::on_btnPresentacion_clicked()
{

}

void MainWindow::on_btnDocumento_clicked()
{

}

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::leerServidor()
{
    QByteArray buffer;
    buffer.resize(tcpCliente->bytesAvailable());
    tcpCliente->read(buffer.data(), buffer.size());
    ui->edtLog->append((QString) buffer);
    qDebug() << QString (buffer);
}

void MainWindow::on_btnMensaje_clicked()
{
    tcpCliente->write(ui->edtMensaje->text().toLatin1().data(), ui->edtMensaje->text().size());
    ui->edtMensaje->clear();
}

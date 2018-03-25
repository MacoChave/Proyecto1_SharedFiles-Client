#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    usuario = "";
    limpiarInformacion();
    ui->frmNuevo->setVisible(false);

    conectar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/***********************************************************************************
 * MANEJO DE CONEXION CLIENTE
 **********************************************************************************/
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
            cargarInformacion();
        }
    }
    else if (mensaje.startsWith("LISTDOCS"))
    {
        if (splMensaje.size() > 1)
        {
            int y = ui->tblDocumentos->rowCount();

            ui->tblDocumentos->insertRow(y);
            ui->tblDocumentos->setItem(y, 0, new QTableWidgetItem(splMensaje[1]));
            ui->tblDocumentos->setItem(y, 1, new QTableWidgetItem(splMensaje[2]));
            ui->tblDocumentos->setItem(y, 2, new QTableWidgetItem(splMensaje[3]));
        }
        else
        {
            QMessageBox msg;
            msg.setWindowTitle("Información");
            msg.setText("El usuario no tiene archivos \n Pulse en 'nuevo' para crear uno");
            msg.exec();
        }
    }
    else if (mensaje.startsWith("INFODOC"))
    {
        pathTemporal = splMensaje[1];
    }
    else if (mensaje.startsWith("CREATEDOCS"))
    {
        if (splMensaje[1].compare("CORRECTO") == 0)
        {
            QMessageBox msg;
            msg.setWindowTitle("Información");
            msg.setText("Documento creado");
            msg.exec();

            limpiarTabla();
            producer("LISTDOCS");
        }
        else
        {
            QMessageBox msg;
            msg.setWindowTitle("Información");
            msg.setText("Hubo un error en el servidor al crear el documento");
            msg.exec();
        }
    }
    else if (mensaje.startsWith("UPDATEDOCS"))
    {
        if (splMensaje[1].compare("CORRECTO") == 0)
        {
            QMessageBox msg;
            msg.setWindowTitle("Información");
            msg.setText("Documento actuallizado");
            msg.exec();

            limpiarTabla();
            producer("LISTDOCS");
        }
        else
        {
            QMessageBox msg;
            msg.setWindowTitle("Información");
            msg.setText("Hubo un error en el servidor al actualizar el documento");
            msg.exec();
        }
    }
    else if (mensaje.startsWith("DELETEDOCS"))
    {
        if (splMensaje[1].compare("CORRECTO") == 0)
        {
            QMessageBox msg;
            msg.setWindowTitle("Información");
            msg.setText("Documento eliminado");
            msg.exec();

            limpiarTabla();
            producer("LISTDOCS");
        }
        else
        {
            QMessageBox msg;
            msg.setWindowTitle("Información");
            msg.setText("Hubo un error en el servidor al eliminar el documento");
            msg.exec();
        }
    }
}

/***********************************************************************************
 * MANEJO DE UI
 **********************************************************************************/
void MainWindow::cargarInformacion()
{
    ui->frmLogin->setVisible(false);
    ui->frmLogout->setVisible(true);
    producer("LISTDOCS");
}

void MainWindow::limpiarInformacion()
{
    ui->frmLogin->setVisible(true);
    ui->frmLogout->setVisible(false);

    limpiarTabla();
}

void MainWindow::limpiarTabla()
{
    ui->tblDocumentos->clear();
    ui->tblDocumentos->setRowCount(0);
}

void MainWindow::on_btnActualizar_clicked()
{
    limpiarInformacion();
    cargarInformacion();
}

/***********************************************************************************
 * MANEJO DE SESION
 **********************************************************************************/
void MainWindow::on_btnLogin_clicked()
{
    DialogLogin l(this);
    l.setWindowTitle("LogIn / LogUp");
    l.setTcp(tcpCliente);

    l.exec();
    producer("SESSION^USUARIO");
}

void MainWindow::on_btnLogout_clicked()
{
    producer("LOGOUT^SESSION");
    ui->lblUsuario->clear();
    usuario.clear();
    limpiarInformacion();
}

/***********************************************************************************
 * MANEJO DEL GESTOR DE DOCUMENTOS
 **********************************************************************************/
void MainWindow::on_btnEliminar_clicked()
{
    if (usuario.isEmpty())
        return;
    on_btnActualizar_clicked();
}

void MainWindow::on_btnVer_clicked()
{
    if (usuario.isEmpty())
        return;
    on_btnActualizar_clicked();
}

void MainWindow::on_btnNuevo_clicked()
{
    if (usuario.isEmpty())
        return;

    ui->frmNuevo->setVisible(true);

    on_btnActualizar_clicked();
}

/***********************************************************************************
 * MANEJO DE DOCUMENTOS, PRESENTACION E IMAGENES
 **********************************************************************************/
void MainWindow::on_btnDocumento_clicked()
{
    DialogDocument d(this);
    d.setTcp(tcpCliente);
    d.setWindowTitle("Editor de documentos");
    d.exec();

    ui->frmNuevo->setVisible(false);
}

void MainWindow::on_btnPresentacion_clicked()
{

}

void MainWindow::on_btnLienzo_clicked()
{

}

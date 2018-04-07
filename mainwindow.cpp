#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    usuario = "";
    clearInformation();
    ui->frmNuevo->setVisible(false);

    connectClient();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/***********************************************************************************
 * MANEJO DE CONEXION CLIENTE
 **********************************************************************************/
void MainWindow::connectClient()
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
    interpreter(QString (buffer));
}

void MainWindow::producer(QString value)
{
    tcpCliente->write(
                        value.toLatin1().data(),
                        value.size()
                    );
}

void MainWindow::interpreter(QString mensaje)
{
    QStringList splMensaje = mensaje.split("^");

    if (mensaje.startsWith("SESSION"))
        actionSesion(splMensaje);
    else if (mensaje.startsWith("LISTFILES"))
        actionListFiles(splMensaje);
    else if (mensaje.startsWith("INFOFILE"))
        actionInfoFile(splMensaje);
    else if (mensaje.startsWith("CREATEFILE"))
        actionCreateFile(splMensaje);
    else if (mensaje.startsWith("UPDATEFILE"))
        actionUpdateFile(splMensaje);
    else if (mensaje.startsWith("DELETEFILE"))
        actionDeleteFile(splMensaje);
}

void MainWindow::actionSesion(QStringList value)
{
    if (value.size() > 1)
    {
        usuario = value[1];
        ui->lblUsuario->setText(usuario);
        loadInformation();
    }
}

void MainWindow::actionListFiles(QStringList value)
{
    if (value.size() > 1)
    {
        for (int i = 1; i <= value.size() - 1; i++)
        {
            if (value[i].isEmpty())
                break;

            int y = ui->tblDocumentos->rowCount();

            ui->tblDocumentos->insertRow(y);
            ui->tblDocumentos->setItem(y, 0, new QTableWidgetItem(value[i++]));
            ui->tblDocumentos->setItem(y, 1, new QTableWidgetItem(value[i++]));
            ui->tblDocumentos->setItem(y, 2, new QTableWidgetItem(value[i]));
        }
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle("Información");
        msg.setText("El usuario no tiene archivos \n Pulse en 'nuevo' para crear uno");
        msg.exec();
    }
}

void MainWindow::actionInfoFile(QStringList value)
{
    pathTemporal = value[1];
}

void MainWindow::actionCreateFile(QStringList value)
{
    if (value[1].compare("CORRECTO") == 0)
    {
        QMessageBox msg;
        msg.setWindowTitle("Información");
        msg.setText("Documento creado");
        msg.exec();

        cleanTable();
        producer("LISTFILES");
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle("Información");
        msg.setText("Hubo un error en el servidor al crear el documento");
        msg.exec();
    }
}

void MainWindow::actionUpdateFile(QStringList value)
{
    if (value[1].compare("CORRECTO") == 0)
    {
        QMessageBox msg;
        msg.setWindowTitle("Información");
        msg.setText("Documento actuallizado");
        msg.exec();

        cleanTable();
        producer("LISTFILES");
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle("Información");
        msg.setText("Hubo un error en el servidor al actualizar el documento");
        msg.exec();
    }
}

void MainWindow::actionDeleteFile(QStringList value)
{
    if (value[1].compare("CORRECTO") == 0)
    {
        QMessageBox msg;
        msg.setWindowTitle("Información");
        msg.setText("Documento eliminado");
        msg.exec();

        cleanTable();
        producer("LISTFILES");
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle("Información");
        msg.setText("Hubo un error en el servidor al eliminar el documento");
        msg.exec();
    }
}

/***********************************************************************************
 * MANEJO DE UI
 **********************************************************************************/
void MainWindow::loadInformation()
{
    ui->frmLogin->setVisible(false);
    ui->frmLogout->setVisible(true);
    producer("LISTFILES");
}

void MainWindow::clearInformation()
{
    ui->frmLogin->setVisible(true);
    ui->frmLogout->setVisible(false);

    cleanTable();
}

void MainWindow::cleanTable()
{
    ui->tblDocumentos->clear();
    ui->tblDocumentos->setRowCount(0);
}

void MainWindow::on_btnActualizar_clicked()
{
    clearInformation();
    loadInformation();
}

/***********************************************************************************
 * MANEJO DE SESION
 **********************************************************************************/
void MainWindow::on_btnLogin_clicked()
{
    DialogLogin l(this);
    l.setWindowTitle("LogIn / LogUp");
    l.exec();

    connectClient();
    producer("SESSION");
}

void MainWindow::on_btnLogout_clicked()
{
    producer("LOGOUT");
    ui->lblUsuario->clear();
    usuario.clear();
    clearInformation();
}

/***********************************************************************************
 * MANEJO DEL GESTOR DE DOCUMENTOS
 **********************************************************************************/
void MainWindow::on_btnEliminar_clicked()
{
    if (usuario.isEmpty())
        return;

    int y = ui->tblDocumentos->currentRow();
    QString filename = ui->tblDocumentos->item(y, 0)->text();
    filename.push_front("DELETEDOCS^");
    producer(filename);
}

void MainWindow::on_btnVer_clicked()
{
    if (usuario.isEmpty())
        return;

    int y = ui->tblDocumentos->currentRow();
    QString filename = ui->tblDocumentos->item(y, 0)->text();
    QString tipo = ui->tblDocumentos->item(y, 1)->text();
    QString permiso = ui->tblDocumentos->item(y, 2)->text();

    tcpCliente = NULL;
    if (tipo.compare("Documento") == 0)
    {
        DialogDocument d(this);
        d.setWindowTitle("Editor de documentos");
        d.setInfo(filename, permiso);
        d.exec();
    }
    else if (tipo.compare("Lienzo") == 0)
    {
        DialogLienzo l(this);
        l.setWindowTitle("Editor de imagenes");
        l.setInfo(filename, permiso);
        l.exec();
    }
    else
    {
        DialogPresentacion p(this);
        p.setWindowTitle("Editor de presentacion");
        p.setInfo(filename, permiso);
        p.exec();
    }

    connectClient();
}

void MainWindow::on_btnNuevo_clicked()
{
    if (usuario.isEmpty())
        return;

    ui->frmNuevo->setVisible(true);
}

/***********************************************************************************
 * MANEJO DE DOCUMENTOS, PRESENTACION E IMAGENES
 **********************************************************************************/
void MainWindow::on_btnDocumento_clicked()
{
    tcpCliente = NULL;
    DialogDocument d(this);
    d.setWindowTitle("Editor de documentos");
    d.exec();

    connectClient();
    on_btnActualizar_clicked();
    ui->frmNuevo->setVisible(false);
}

void MainWindow::on_btnPresentacion_clicked()
{
    tcpCliente = NULL;
    
    DialogPresentacion p(this);
    p.setWindowTitle("Editor de presentaciones");
    p.setInfo("", "Propietario");
    p.exec();

    connectClient();
    on_btnActualizar_clicked();
    ui->frmNuevo->setVisible(false);
}

void MainWindow::on_btnLienzo_clicked()
{
    tcpCliente = NULL;

    DialogLienzo l(this);
    l.setWindowTitle("Editor de imagenes");
    l.exec();

    connectClient();
    on_btnActualizar_clicked();
    ui->frmNuevo->setVisible(false);
}

#include "dialogpresentacion.h"
#include "ui_dialogpresentacion.h"

DialogPresentacion::DialogPresentacion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPresentacion)
{
    ui->setupUi(this);
    usuario = "";
    filename = "";
    permiso = "";
    lista = new List<TADList *>();

    conectar();
}

DialogPresentacion::~DialogPresentacion()
{
    delete ui;
}

void DialogPresentacion::setInfo(QString _filename, QString _permiso, int layout)
{
    filename = _filename;
    permiso = _permiso;

    switch (layout) {
    case 0:
        on_btnTitulo_clicked();
        break;
    case 1:
        on_btnCompleta_clicked();
        break;
    case 2:
        on_btnDoble_clicked();
        break;
    }

    if (!filename.isEmpty())
    {
        QString mensaje("INFOPPT^");
        mensaje.append(filename);

        producer(mensaje);
    }
}

void DialogPresentacion::conectar()
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

void DialogPresentacion::consumer()
{
    QByteArray buffer;
    buffer.resize(tcpCliente->bytesAvailable());
    tcpCliente->read(buffer.data(), buffer.size());
    interpretarMensaje(QString (buffer));
}

void DialogPresentacion::producer(QString)
{
    tcpCliente->write(
                            value.toLatin1().data(),
                            value.size()
                    );
}

void DialogPresentacion::interpreter(QString mensaje)
{
    QStringList splMensaje = mensaje.split("^");
    qDebug() << splMensaje;

    if (mensaje.startsWith("INFOPPT"))
    {
        if (splMensaje.size() > 1)
        {
            jsd = QJsonDocument::fromJson(splMensaje[2].toUtf8());
            cargarLista();
        }
    }
}

void DialogPresentacion::cargarLista()
{
    if (jsd->isEmpty())
    {
        qDebug() << "El fichero JSON está vacío";
        return;
    }

    QJsonArray jsa = jsd->array();
    for (int i = 0; i < jsa.count(); i++)
    {
        QJsonObject jso = jsa.at(i).toObject();
        TADList *tadLista = new TADList();
        tadLista->setTitulo(jso["titulo"].toString());

        QString layout = jso["layout"].toString();
        if (layout.compare("titulo") == 0)
        {
            tadLista->setLayout(tadLista->TITULO);
        }
        else if (layout.compare("completa") == 0)
        {
            tadLista->setLayout(tadLista->COMPLETO);
            tadLista->setContenido(jso["contenido"].toString());
            tadLista->setImagen1(jso["imagen"].toString());
        }
        else if (layout.compare("doble") == 0)
        {
            tadLista->setLayout(tadLista->DOBLE);
            tadLista->setImagen1(jso["imagen1"].toString());
            tadLista->setImagen2(jso["imagen2"].toString());
        }

        lista->push_back(tadLista);
    }
}

void DialogPresentacion::on_btnTitulo_clicked()
{
    ui->lblImagen1->setVisible(false);
    ui->edtImagen1->setVisible(false);
    ui->btnImagen1->setVisible(false);

    ui->lblImagen2->setVisible(false);
    ui->edtImagen2->setVisible(false);
    ui->btnImagen2->setVisible(false);

    ui->lblContenido->setVisible(false);
    ui->edtContenido->setVisible(false);
}

void DialogPresentacion::on_btnCompleta_clicked()
{
    ui->lblImagen2->setVisible(false);
    ui->edtImagen2->setVisible(false);
    ui->btnImagen2->setVisible(false);
}

void DialogPresentacion::on_btnDoble_clicked()
{
    ui->lblContenido->setVisible(false);
    ui->edtContenido->setVisible(false);
}

void DialogPresentacion::on_btnNuevo_clicked()
{

}

void DialogPresentacion::on_btnEliminar_clicked()
{

}

void DialogPresentacion::on_btnGuardar_clicked()
{

}

void DialogPresentacion::on_btnImagen1_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                "Selector de imagen",
                "/home/marco/Escritorio/",
                "Archivos JPEG (*.jpg) ; Archivos PNG (*.png)"
                );
    ui->edtImagen1->setText(filename);
}

void DialogPresentacion::on_btnImagen2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                "Selector de imagen",
                "/home/marco/Escritorio/",
                "Archivos JPEG (*.jpg) ; Archivos PNG (*.png)"
                );
    ui->edtImagen2->setText(filename);
}

void DialogPresentacion::on_btnPreview_clicked()
{

}

void DialogPresentacion::on_btnNext_clicked()
{

}

void DialogPresentacion::on_btnGuardar_2_clicked()
{

}

void DialogPresentacion::on_btnCancelar_clicked()
{

}

void DialogPresentacion::on_btnGrafico_clicked()
{
    lista->graph();
}

void DialogPresentacion::on_btnPDF_clicked()
{

}

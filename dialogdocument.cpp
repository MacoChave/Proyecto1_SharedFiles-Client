#include "dialogdocument.h"
#include "ui_dialogdocument.h"

DialogDocument::DialogDocument(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDocument)
{
    ui->setupUi(this);
    tree = new GenericTree<NodeGenericTree *>();
    filename = "";
    opcion = 0;
    count = 0;

    conectar();
}

DialogDocument::~DialogDocument()
{
    delete ui;
    delete tree;
}

void DialogDocument::setTcp(QTcpSocket *value)
{
    tcpCliente = value;
}

void DialogDocument::setInfo(QString _filename, QString _permiso)
{
    filename = _filename;
    if (_permiso.compare("Ver") == 0)
    {
        ui->btnGuardarNodo->setVisible(false);
        ui->btnEliminar->setVisible(false);

        ui->btnGuardar->setVisible(false);
        ui->btnNuevo->setVisible(false);
        ui->btnEditar->setVisible(false);
    }
    else if (_permiso.compare("Editar") == 0)
    {
    }

    if (!filename.isEmpty())
    {
        QString msj("INFOFILE^");
        msj.append(filename);
        producer(msj);
    }
}

/***********************************************************************************
 * MANEJO DE CONEXION CLIENTE
 **********************************************************************************/
void DialogDocument::conectar()
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

void DialogDocument::consumer()
{
    QByteArray buffer;
    buffer.resize(tcpCliente->bytesAvailable());
    tcpCliente->read(buffer.data(), buffer.size());
    interpreter(QString (buffer));
}

void DialogDocument::producer(QString value)
{
    tcpCliente->write(
                        value.toLatin1().data(),
                        value.size()
                );
}

void DialogDocument::interpreter(QString mensaje)
{
    QStringList splMensaje = mensaje.split("^");
    qDebug() << "DOCUMENTO " << splMensaje;

    if (mensaje.startsWith("INFOFILE"))
        actionInfoFile(splMensaje);
}

void DialogDocument::actionInfoFile(QStringList value)
{
    if (value.size() > 1)
    {
        jsd = QJsonDocument::fromJson(value[1].toUtf8());
        cargarArbol();
    }
}

/***********************************************************************************
 * MANEJO DE ARBOL
 **********************************************************************************/
void DialogDocument::cargarArbol()
{
    if (jsd.isEmpty())
    {
        qDebug() << "El fichero JSON está vacío";
        return;
    }

    NodeGenericTree *rootNode = NULL;

    QJsonObject rootJSO = jsd.object();

    QString rootTitle = rootJSO["titulo"].toString();
    QString rootBody = rootJSO["contenido"].toString();
    QJsonArray rootJSA = rootJSO["subsecciones"].toArray();
    TADGenericTree *tadRoot = new TADGenericTree(count, rootTitle, rootBody);
    tadRoot->setTipo(tadRoot->PARRAFO);
    count++;

    rootNode = new NodeGenericTree(tadRoot);
    List<NodeGenericTree *> *rootChilds = NULL;
    rootChilds = obtenerHijos(rootJSA);
    rootNode->setChilds(rootChilds);

    tree->setRoot(rootNode);
}

List<NodeGenericTree *> *DialogDocument::obtenerHijos(QJsonArray currentJSA)
{
    List<NodeGenericTree *> *list = new List<NodeGenericTree *>();

    for (int i = 0; i < currentJSA.count(); i++)
    {
        QString title;
        QString body;
        TADGenericTree *tadNode = NULL;
        NodeGenericTree *node = NULL;

        if (currentJSA.at(i).isObject())
        {
            QJsonObject jso = currentJSA.at(i).toObject();
            title = jso["titulo"].toString();
            body = jso["contenido"].toString();
            QJsonArray jsa = jso["subsecciones"].toArray();
            tadNode = new TADGenericTree(count, title, body);
            count++;

            if (title.startsWith("Imagen:"))
                tadNode->setTipo(tadNode->IMAGEN);
            else
            {
                if (body.isEmpty())
                {
                    tadNode->setTipo(tadNode->VINETAS);
                    jsa = jso["contenido"].toArray();
                }
                else
                    tadNode->setTipo(tadNode->PARRAFO);
            }

            node = new NodeGenericTree(tadNode);

            if (!jsa.isEmpty())
            {
                List<NodeGenericTree *> *childs = NULL;
                childs = obtenerHijos(jsa);
                node->setChilds(childs);
            }
        }
        else
        {
            body = currentJSA.at(i).toString();
            tadNode = new TADGenericTree(count++, body);
            tadNode->setTipo(tadNode->VINETAS);
            node = new NodeGenericTree(tadNode);
        }

        list->push_back(node);
    }
    return list;
}

/***********************************************************************************
 * MANEJO DE METODOS
 **********************************************************************************/
void DialogDocument::on_btnGuardar_clicked()
{

}

void DialogDocument::on_btnCancelar_clicked()
{
    QString peticion("INFOFILE^");
    peticion.append(filename);
    producer(peticion);
    qDebug() << "PEDIR DOCUMENTO " << peticion;
}

void DialogDocument::on_btnNuevo_clicked()
{

}

void DialogDocument::on_btnEditar_clicked()
{

}

void DialogDocument::on_btnPDF_clicked()
{

}

void DialogDocument::on_btnGrafico_clicked()
{
    QString text = tree->graph();

    QFile file("arbolito.dot");
    if (!file.open(QFile::WriteOnly))
        return;

    QTextStream out(&file);
    out << text;
    flush(out);
    file.close();
}

void DialogDocument::on_btnEliminar_clicked()
{

}

void DialogDocument::on_btnGuardarNodo_clicked()
{
    NodeGenericTree *node = NULL;
    TADGenericTree *tad = new TADGenericTree;
    tad->setTitulo(ui->edtTitulo->text());

    switch (opcion) {
    case 1: // PARRAFO
    {
        tad->setContenido(ui->edtContenido->document()->toPlainText());
        node = new NodeGenericTree(tad);
        break;
    }
    case 2: // VIÑETAS
    {
        tad->setContenido("...");
        node = new NodeGenericTree();
        node->setData(tad);
        qDebug() << tad->getContenido();
        break;
    }
    case 3: // IMAGEN
    {
        tad->setContenido(ui->edtImagen->text());
        node = new NodeGenericTree(tad);
    }
    default:
        delete tad;
        tad = NULL;
        return;
        break;
    }
}

void DialogDocument::on_btnParrafo_clicked()
{
    opcion = 1;

    ui->lblTitulo->setVisible(true);
    ui->lblTitulo->setText("Titulo (Opcional)");
    ui->edtImagen->setVisible(true);

    ui->lblContenido->setVisible(true);
    ui->lblContenido->setText("Contenido (Opcional)");
    ui->edtContenido->setVisible(true);

    ui->lblImagen->setVisible(false);
    ui->edtImagen->setVisible(false);
    ui->btnChoose->setVisible(false);
}

void DialogDocument::on_btnVineta_clicked()
{
    opcion = 2;

    ui->lblTitulo->setVisible(true);
    ui->lblTitulo->setText("Titulo (Descripción de la lista de elementos)");
    ui->edtImagen->setVisible(true);

    ui->lblContenido->setVisible(true);
    ui->lblContenido->setText("Ítems (Un elemento por cada línea)");
    ui->edtContenido->setVisible(true);

    ui->lblImagen->setVisible(false);
    ui->edtImagen->setVisible(false);
    ui->btnChoose->setVisible(false);
}

void DialogDocument::on_btnImagen_clicked()
{
    opcion = 3;

    ui->lblTitulo->setVisible(true);
    ui->lblTitulo->setText("Titulo (Pie de la imagen)");
    ui->edtImagen->setVisible(true);

    ui->lblContenido->setVisible(false);
    ui->edtContenido->setVisible(false);

    ui->lblImagen->setVisible(true);
    ui->lblImagen->setText("Imagen");
    ui->edtImagen->setVisible(true);
    ui->btnChoose->setVisible(true);
}

void DialogDocument::on_btnChoose_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                "Seleccion de imagen",
                "/home/marco/Escritorio/",
                "Archivo de imagen (*.jpg) | Archivo de imagen (*.png)"
                );
    ui->edtImagen->setText(filename);
}

#include "dialogdocument.h"
#include "ui_dialogdocument.h"

DialogDocument::DialogDocument(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDocument)
{
    ui->setupUi(this);
    filename = "";
    permiso = "";
    opcion = 0;
    count = 0;
    tree = new GenericTree<NodeGenericTree *>();
    currentItem = NULL;

    connectClient();
}

DialogDocument::~DialogDocument()
{
    delete ui;
    delete tree;
}

void DialogDocument::setInfo(QString _filename, QString _permiso)
{
    filename = _filename;
    permiso = _permiso;

    if (!filename.isEmpty())
    {
        QString msj("INFOFILE^");
        msj.append(filename);

        producer(msj);
    }
    else
    {
        TADGenericTree *tadNodeTree = new TADGenericTree();
        tadNodeTree->setTipo(TADGenericTree::PARRAFO);
        NodeGenericTree *node = new NodeGenericTree(tadNodeTree);
        tree->setRoot(node);

        currentItem = tree->getRoot();
        setData();
    }
}

/***********************************************************************************
 * MANEJO DE CONEXION CLIENTE
 **********************************************************************************/
void DialogDocument::connectClient()
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
    else if (mensaje.startsWith("CODER"))
        actionCoderImage(splMensaje);
}

/***********************************************************************************
 * MANEJO DE ACCIONES
 **********************************************************************************/
void DialogDocument::actionInfoFile(QStringList value)
{
    if (value.size() > 1)
    {
        jsd = QJsonDocument::fromJson(value[1].toUtf8());
        loadTree();

        if (tree->getRoot() != NULL)
        {
            currentItem = tree->getRoot();
            setData();
        }
    }
}

void DialogDocument::actionCoderImage(QStringList value)
{
    QPixmap pixmap;
    if (pixmap.loadFromData(value[1].toLatin1(), "PNG"))
        qDebug() << "QPixmap seteado";
}

/***********************************************************************************
 * MANEJO DE ARCHIVO JSON
 **********************************************************************************/
void DialogDocument::loadTree()
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
    tadRoot->setTipo(TADGenericTree::PARRAFO);
    count++;

    rootNode = new NodeGenericTree(tadRoot);
    List<NodeGenericTree *> *rootChilds = NULL;
    rootChilds = loadChildsTree(rootJSA);
    rootNode->setChilds(rootChilds);

    tree->setRoot(rootNode);
}

List<NodeGenericTree *> *DialogDocument::loadChildsTree(QJsonArray currentJSA)
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
                childs = loadChildsTree(jsa);
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

QString DialogDocument::createJSON()
{
    QJsonObject rootJSO;

    rootJSO.insert("titulo", currentItem->getData()->getTitulo());
    rootJSO.insert("contenido", currentItem->getData()->getContenido());

    QJsonArray jsa = getChildsTree(currentItem->getChilds());
    rootJSO.insert("subsecciones", jsa);
}

QJsonArray DialogDocument::getChildsTree(List<NodeGenericTree *> *currentList)
{
    QJsonArray rootJSA;

    Node<NodeGenericTree *> *node = currentList->first();
    while (node != NULL)
    {
        NodeGenericTree *nodeTree = node->getData();

        QJsonObject rootJSO;
        rootJSO.insert("titulo", nodeTree->getData()->getTitulo());
        rootJSO.insert("contenido", nodeTree->getData()->getContenido());

        if (nodeTree->getChilds() != NULL)
        {
            if (nodeTree->getData()->getTipo() == TADGenericTree::VINETAS)
            {
            }
            else
            {
                QJsonArray jsa = getChildsTree(nodeTree->getChilds());
                rootJSO.insert("subsecciones", jsa);
            }
        }

        rootJSA.push_back(rootJSO);

        node = node->getNext();
    }

    return rootJSA;
}

void DialogDocument::setData()
{
    if (currentItem == NULL)
        return;
}

void DialogDocument::updateCurrentData()
{

}

QString DialogDocument::coderImage(QString value)
{
    QByteArray ba = value.toLatin1();

    return QString (ba.toBase64());
}

void DialogDocument::decoderImage(QString value)
{
    producer("CODER^" + value);
}

/***********************************************************************************
 * MANEJO DE DISEÑO DE DOCUMENTO
 **********************************************************************************/
void DialogDocument::on_btnParrafo_clicked()
{
    currentItem->getData()->setTipo(TADGenericTree::PARRAFO);

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
    currentItem->getData()->setTipo(TADGenericTree::VINETAS);

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
    currentItem->getData()->setTipo(TADGenericTree::IMAGEN);

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

/***********************************************************************************
 * MANEJO DE ITEM
 **********************************************************************************/
void DialogDocument::on_btnNuevo_clicked()
{
    TADGenericTree *tad = new TADGenericTree();
    tad->setTipo(TADGenericTree::PARRAFO);
    NodeGenericTree *node = new NodeGenericTree(tad);

    currentItem->addChild(node);

    on_btnParrafo_clicked();
    currentItem = node;
    setData();
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

void DialogDocument::on_btnGrafico_clicked()
{

}

void DialogDocument::on_btnPDF_clicked()
{

}

/**
 * @brief DialogPresentacion::getX
 * @param Indice del 0 - 12
 * @return Coordenada x
 */
int DialogDocument::getX(int i)
{
    return 50 * i;
}

/**
 * @brief DialogPresentacion::getY
 * @param Indice del 0 - 6
 * @return Coordenada y
 */
int DialogDocument::getY(int j)
{
    return 50 * j;
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

/***********************************************************************************
 * CAPTURAR TEXTO
 **********************************************************************************/
void DialogDocument::on_edtTitulo_returnPressed()
{
    currentItem->getData()->setTitulo(ui->edtTitulo->text());
}

void DialogDocument::on_edtContenido_textChanged()
{
    currentItem->getData()->setContenido(ui->edtContenido->toPlainText());
}

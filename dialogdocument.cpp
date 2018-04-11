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
        else
        qDebug() << "Arbol vacío";
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
    qDebug() << "Arbol cargado";
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
    producer("CODER^" + value);
}

QPixmap DialogDocument::decoderImage(QString value)
{
    QByteArray ba;
    ba = QByteArray::fromBase64(value.toLatin1());
    QPixmap pixmap;
    pixmap.loadFromData(ba);

    return pixmap;
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
    QString text;
    text.append("digraph arbolito");
    text.append(" {\n");
    text.append("node [shape = \"box\"]\n");

    text.append(tree->graph());

    text.append("}");

    QFile file("arbolito.dot");
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        out << text;

        flush(out);
        file.close();
    }
}

void DialogDocument::on_btnPDF_clicked()
{
    if (filename.isEmpty())
        on_btnGuardar_clicked();

    QString filepath(filename);
    filepath.append(".pdf");

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filepath);
    printer.setOrientation(QPrinter::Portrait);
    printer.setPaperSize(QPrinter::Letter);

    QPainter painter;

    if (!painter.begin(&printer))
    {
        qWarning("Failed to open file, is it writable?");
        return;
    }

    int y = 1;
    int fromX = getX(0);
    int fromY = getY(y);
    int toX = getX(15);
    int toY = getY(y);

    QString titulo = currentItem->getData()->getTitulo();
    QString contenido = currentItem->getData()->getContenido();
    qDebug() << "Titulo -> " << titulo;
    qDebug() << "Contenido.size -> " << contenido.size();
    qDebug() << "Contenido -> " << contenido;

    painter.setFont(QFont("Arial", 18, 2));
    painter.drawText(QRect(fromX, fromY, toX - fromX, getY(1)), Qt::AlignHCenter, titulo);
    y++;

    int divisiones = contenido.size() / 50;
    fromY = getY(y);
    toY = getY(y + divisiones);

    painter.setFont(QFont("Arial", 12));
    painter.drawText(QRect(fromX, fromY, toX - fromX, toY - fromY), Qt::TextWordWrap, contenido);
    y += divisiones;

    Node<NodeGenericTree *> *item = currentItem->getChilds()->first();

    while (item != NULL)
    {
        childPDF(painter, item->getData(), y, 1);

        item = item->getNext();
    }

    painter.end();
    qDebug() << "Creación de pdf completo";
    QString cmd("xdg-open ");
    cmd.append(filepath);

    system(cmd.toLatin1().data());
}

void DialogDocument::childPDF(QPainter &painter, NodeGenericTree *current, int &y, int level)
{
    TADGenericTree *tad = current->getData();
    int fromX = getX(0);
    int toX = getX(15);
    int fromY = getY(y);
    int toY;

    QString titulo;
    QString contenido;

    switch (tad->getTipo())
    {
        case TADGenericTree::PARRAFO:
        {
            titulo = tad->getTitulo();
            contenido = tad->getContenido();

            if (level > 3)
                painter.setFont(QFont("Arial", 18 - 2*level, 2));
            else
                painter.setFont(QFont("Arial", 12));

            painter.drawText(QRect(fromX, fromY, toX - fromX, fromY), Qt::AlignLeft, titulo);
            y++;

            int divisiones = contenido.size() / 50;
            fromY = getY(y);
            toY = getY(y + divisiones);

            painter.setFont(QFont("Arial", 12));
            painter.drawText(QRect(fromX, fromY, toX - fromX, toY - fromY), Qt::TextWordWrap, contenido);
            y += divisiones;

            break;
        }
        case TADGenericTree::VINETAS:
        {
            titulo = tad->getTitulo();
            contenido = tad->getContenido();

            Node<NodeGenericTree *> *child = current->getChilds()->first();

            while (child != NULL)
            {
                contenido = child->getData()->getData()->getContenido();
                int divisiones = contenido.size() / 50;
                fromY = getY(y);
                toY = getY(y + divisiones);

                painter.drawText(QRect(fromX, fromY, toX - fromX, toY - fromY), Qt::TextWordWrap, contenido);
                y += divisiones;

                child = child->getNext();
            }
            return;

            break;
        }
        case TADGenericTree::IMAGEN:
        {
            QPixmap pixmap;
            titulo = tad->getTitulo();
            contenido = tad->getContenido();

            fromX = getX(3);
            toX = getX(12);
            fromY = getY(y);
            toY = getY(y + 8);
            pixmap = decoderImage(contenido);
            painter.drawPixmap(QRect(fromX, fromY, toX - fromX, toY - fromY), pixmap);
            y += 9;

            painter.setFont(QFont("Arial", 12));
            painter.drawText(QRect(fromX, fromY, toX - fromX, fromY), Qt::AlignHCenter, titulo);
            y++;

            break;
        }
    }

    if (current->getChilds() == NULL)
        return;

    Node<NodeGenericTree *> *item = current->getChilds()->first();

    while (item != NULL)
    {
        childPDF(painter, item->getData(), y, 1);

        item = item->getNext();
    }
}

/**
 * @brief DialogPresentacion::getX
 * @param Indice del 0 - 12
 * @return Coordenada x
 */
int DialogDocument::getX(int i)
{
    return (50 * i) + 10;
}

/**
 * @brief DialogPresentacion::getY
 * @param Indice del 0 - 6
 * @return Coordenada y
 */
int DialogDocument::getY(int j)
{
    return (50 * j) + 5;
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

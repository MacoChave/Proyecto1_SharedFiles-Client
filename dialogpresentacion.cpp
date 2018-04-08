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
    currentNode = NULL;

    connectClient();
}

DialogPresentacion::~DialogPresentacion()
{
    delete ui;
}

void DialogPresentacion::setInfo(QString _filename, QString _permiso)
{
    filename = _filename;
    permiso = _permiso;

    if (!filename.isEmpty())
    {
        QString mensaje("INFOFILE^");
        mensaje.append(filename);

        producer(mensaje);
    }
    else
    {
        TADList *tadList = new TADList();
        tadList->setLayout(TADList::TITULO);
        lista->push_back(tadList);

        currentNode = lista->first();
        setData();
    }
}

/***********************************************************************************
 * MANEJO DE CONEXION CLIENTE
 **********************************************************************************/
void DialogPresentacion::connectClient()
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
    interpreter(QString (buffer));
}

void DialogPresentacion::producer(QString value)
{
    tcpCliente->write(
                            value.toLatin1().data(),
                            value.size()
                    );
}

void DialogPresentacion::interpreter(QString message)
{
    QStringList splMensaje = message.split("^");
    qDebug() << splMensaje;

    if (message.startsWith("INFOFILE"))
        actionInfoFile(splMensaje);
    else if (message.startsWith("CODER"))
        actionCoderImage(splMensaje);
}

/***********************************************************************************
 * MANEJO DE ACCIONES
 **********************************************************************************/
void DialogPresentacion::actionInfoFile(QStringList value)
{
    if (value.size() > 1)
    {
        jsd = QJsonDocument::fromJson(value[1].toUtf8());
        if (loadList())
        {
            currentNode = lista->first();
            setData();
            ui->btnPreview->setEnabled(false);
        }
    }
}

void DialogPresentacion::actionCoderImage(QStringList value)
{
    QByteArray ba = QByteArray::fromBase64(value[1].toLatin1());
    QPixmap pixmap;
    if (pixmap.loadFromData(ba))
        qDebug() << "QPixmap seteado";
}

/***********************************************************************************
 * MANEJO DE ARCHIVO JSON
 **********************************************************************************/
bool DialogPresentacion::loadList()
{
    if (jsd.isEmpty())
    {
        qDebug() << "El fichero JSON está vacío";
        return false;
    }

    QJsonArray jsa = jsd.array();
    for (int i = 0; i < jsa.count(); i++)
    {
        QJsonObject jso = jsa.at(i).toObject();
        TADList *tadLista = new TADList();
        tadLista->setTitulo(jso["titulo"].toString());
        tadLista->setId(lista->size());

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

        qDebug() << "Item agregado " << tadLista->getTitulo();
    }

    return true;
}

QString DialogPresentacion::createJSON()
{
    Node<TADList *> *temporalNode;
    temporalNode = lista->first();
    QJsonArray rootJSA;

    while (temporalNode != NULL)
    {
        QJsonObject jso;

        jso.insert("titulo", QJsonValue::fromVariant(temporalNode->getData()->getTitulo()));

        int layout = temporalNode->getData()->getlayout();

        switch (layout) {
        case TADList::TITULO:
            jso.insert("layout", QJsonValue::fromVariant("titulo"));
            break;
        case TADList::COMPLETO:
            jso.insert("layout", QJsonValue::fromVariant("completa"));
            jso.insert("contenido", QJsonValue::fromVariant(temporalNode->getData()->getContenido()));
            jso.insert("imagen", QJsonValue::fromVariant(temporalNode->getData()->getImagen1()));
            break;
        case TADList::DOBLE:
            jso.insert("layout", QJsonValue::fromVariant("doble"));
            jso.insert("imagen1", QJsonValue::fromVariant(temporalNode->getData()->getImagen1()));
            jso.insert("imagen2", QJsonValue::fromVariant(temporalNode->getData()->getImagen2()));
            break;
        }

        rootJSA.push_back(jso);

        temporalNode = temporalNode->getNext();
    }

    jsd.setArray(rootJSA);

    QString strJSON(jsd.toJson());

    return strJSON;
}

void DialogPresentacion::setData()
{
    TADList *currentTadList = currentNode->getData();

    ui->edtTitulo->setText(currentTadList->getTitulo());
    ui->edtContenido->setText(currentTadList->getContenido());
    ui->edtImagen1->setText(currentTadList->getImagen1());
    ui->edtImagen2->setText(currentTadList->getImagen2());

    if (currentTadList->getlayout() == TADList::TITULO)
        on_btnTitulo_clicked();
    else if (currentTadList->getlayout() == TADList::COMPLETO)
        on_btnCompleta_clicked();
    else if (currentTadList->getlayout() == TADList::DOBLE)
        on_btnDoble_clicked();
}

void DialogPresentacion::updateCurrentData()
{
    currentNode->getData()->setTitulo(ui->edtTitulo->text());
    currentNode->getData()->setContenido(ui->edtContenido->toPlainText());
    currentNode->getData()->setImagen1(ui->edtImagen1->text());
    currentNode->getData()->setImagen2(ui->edtImagen2->text());
}

QString DialogPresentacion::coderImage(QString value)
{
    QByteArray ba = value.toLatin1();

    return QString (ba.toBase64());
}

void DialogPresentacion::decoderImage(QString value)
{
    producer("CODER^" + value);
}

/***********************************************************************************
 * MANEJO DE DISEÑO DE DIAPOSITIVA
 **********************************************************************************/
void DialogPresentacion::on_btnTitulo_clicked()
{
    currentNode->getData()->setLayout(TADList::TITULO);

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
    currentNode->getData()->setLayout(TADList::COMPLETO);

    ui->lblImagen1->setVisible(true);
    ui->edtImagen1->setVisible(true);
    ui->btnImagen1->setVisible(true);

    ui->lblImagen2->setVisible(false);
    ui->edtImagen2->setVisible(false);
    ui->btnImagen2->setVisible(false);

    ui->lblContenido->setVisible(true);
    ui->edtContenido->setVisible(true);
}

void DialogPresentacion::on_btnDoble_clicked()
{
    currentNode->getData()->setLayout(TADList::DOBLE);

    ui->lblImagen1->setVisible(true);
    ui->edtImagen1->setVisible(true);
    ui->btnImagen1->setVisible(true);

    ui->lblImagen2->setVisible(true);
    ui->edtImagen2->setVisible(true);
    ui->btnImagen2->setVisible(true);

    ui->lblContenido->setVisible(false);
    ui->edtContenido->setVisible(false);
}

/***********************************************************************************
 * MANEJO DE DIAPOSITIVA
 **********************************************************************************/
void DialogPresentacion::on_btnNuevo_clicked()
{
    Node<TADList *> *node = new Node<TADList *>(new TADList());
    node->setNext(currentNode->getNext());
    node->setPreview(currentNode);
    if (currentNode->getNext() != NULL)
        currentNode->getNext()->setPreview(node);
    currentNode->setNext(node);

    node->getData()->setLayout(TADList::TITULO);
    on_btnTitulo_clicked();
    currentNode = node;
    setData();
}

void DialogPresentacion::on_btnEliminar_clicked()
{
    if (lista->size() < 2)
        return;

    Node<TADList *> *temporal = currentNode;
    if (temporal->getPreview() != NULL)
        temporal->getPreview()->setNext(temporal->getNext());
    if (temporal->getNext() != NULL)
        temporal->getNext()->setPreview(temporal->getPreview());

    if (temporal->getNext() != NULL)
        currentNode = temporal->getNext();
    else
        currentNode = temporal->getPreview();

    if (currentNode->getNext() == NULL)
        ui->btnNext->setEnabled(false);
    if (currentNode->getPreview() == NULL)
        ui->btnPreview->setEnabled(false);

    delete temporal;
    temporal = NULL;
    setData();
}

/***********************************************************************************
 * MANEJO DE CONTENIDO DE DIAPOSITIVA
 **********************************************************************************/
void DialogPresentacion::on_btnImagen1_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                "Selector de imagen",
                "/home/marco/Escritorio/",
                "Archivos JPEG (*.jpg) ; Archivos PNG (*.png)"
                );
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray image = file.readAll();
        file.close();

        QString encoded = QString(image.toBase64());

        currentNode->getData()->setImagen1(encoded);
    }
    else
        currentNode->getData()->setImagen1("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");

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
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray image = file.readAll();
        file.close();

        QString encoded = QString(image.toBase64());

        currentNode->getData()->setImagen2(encoded);
    }
    else
        currentNode->getData()->setImagen1("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");

    ui->edtImagen2->setText(filename);
}

/***********************************************************************************
 * MANEJO DE FLUJO DE DIAPOSITIVAS
 **********************************************************************************/
void DialogPresentacion::on_btnPreview_clicked()
{
    updateCurrentData();
    currentNode = currentNode->getPreview();
    setData();

    if (currentNode->getPreview() == NULL)
        ui->btnPreview->setEnabled(false);
    if (currentNode->getNext() != NULL)
        ui->btnNext->setEnabled(true);
}

void DialogPresentacion::on_btnNext_clicked()
{
    updateCurrentData();
    currentNode = currentNode->getNext();
    setData();

    if (currentNode->getNext() == NULL)
        ui->btnNext->setEnabled(false);
    if (currentNode->getPreview() != NULL)
        ui->btnPreview->setEnabled(true);
}

/***********************************************************************************
 * MANEJO DE PRESENTACION
 **********************************************************************************/
void DialogPresentacion::on_btnGuardar_clicked()
{
    QString tipo("Presentacion");
    QString contenido;
    QString mensaje;
    contenido = createJSON();

    if (filename.isEmpty())
    {
        bool ok;
        filename = QInputDialog::getText(
                    this,
                    tr("Nombre del archivo"),
                    tr("Filename:"),
                    QLineEdit::Normal,
                    QDir::home().dirName(),
                    &ok
                    );

        if (!ok || filename.isEmpty())
            return;

        mensaje.append("CREATEFILE^");
    }
    else
        mensaje.append("UPDATEFILE^");

    mensaje.append(filename);
    mensaje.append("^");
    mensaje.append(permiso);
    mensaje.append("^");
    mensaje.append(tipo);
    mensaje.append("^");
    mensaje.append(contenido);

    producer(mensaje);
}

void DialogPresentacion::on_btnCancelar_clicked()
{

}

void DialogPresentacion::on_btnGrafico_clicked()
{
    QString contenido = lista->graph();
    QFile file("listita.dot");
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        out << "digraph listita {\n";
        out << "node [shape = \"box\"]\n";
        out << "rankdir = LR;\n";
        flush(out);
        out << contenido;
        flush(out);
        out << "\n}\n";
        flush(out);

        file.close();
        system("dot -Tpng listita.dot -o listita.png");
    }
}

void DialogPresentacion::on_btnPDF_clicked()
{
    if (filename.isEmpty())
        on_btnGuardar_clicked();

    Node<TADList *> *temporal = lista->first();
    QString filepath(filename);
    filepath.append(".pdf");

    int y = 20;

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filepath);
    printer.setOrientation(QPrinter::Landscape);
    printer.setPaperSize(QPrinter::Letter);

    QPainter painter;

    if (!painter.begin(&printer))
    {
        qWarning("Failed to open file, is it writable?");
        return;
    }

    while (temporal != NULL)
    {
        int layout = temporal->getData()->getlayout();
        int fromX = 0;
        int toX = 0;
        int fromY = 0;
        int toY = 0;

        switch (layout)
        {
            case TADList::TITULO:
            {
                painter.setFont(QFont("Liberation Sans", 44));
                fromX = getX(1);
                toX = getX(9);
                fromY = getY(6);
                toY = getY(8);
                painter.drawText(QRect(fromX, fromY, toX - fromX, toY - fromY), Qt::AlignCenter, temporal->getData()->getTitulo());
                qDebug() << temporal->getData()->getTitulo();

                break;
            }
            case TADList::COMPLETO:
            {
                painter.setFont(QFont("Liberation Sans", 44));
                fromX = getX(1);
                toX = getX(9);
                fromY = getY(2);
                toY = getY(3);
                painter.drawText(QRect(fromX, fromY, toX - fromX, toY - fromY), Qt::AlignCenter, temporal->getData()->getTitulo());
                qDebug() << temporal->getData()->getTitulo();

                painter.setFont(QFont("Liberation Sans", 32));
                fromX = getX(1);
                toX = getX(9);
                fromY = getY(5);
                toY = getY(14);
                painter.drawText(QRect(fromX, fromY, toX - fromX, toY - fromY), Qt::TextWordWrap, temporal->getData()->getContenido());
                qDebug() << temporal->getData()->getContenido();

                break;
            }
            case TADList::DOBLE:
            {
                painter.setFont(QFont("Liberation Sans", 44));
                fromX = getX(1);
                toX = getX(9);
                fromY = getY(2);
                toY = getY(3);
                painter.drawText(QRect(fromX, fromY, toX - fromX, toY - fromY), Qt::AlignCenter, temporal->getData()->getTitulo());
                qDebug() << temporal->getData()->getTitulo();

                fromX = getX(1);
                toX = getX(4);
                fromY = getY(5);
                toY = getY(14);
                painter.drawText(QRect(fromX, fromY, toX - fromX, toY - fromY), Qt::AlignCenter, temporal->getData()->getImagen1());

                fromX = getX(6);
                toX = getX(9);
                fromY = getY(5);
                toY = getY(14);
                painter.drawText(QRect(fromX, fromY, toX - fromX, toY - fromY), Qt::AlignCenter, temporal->getData()->getImagen2());

                break;
            }
        }

        if (!printer.newPage())
        {
            qWarning("failed in flushing page to disk, disk full?");
            break;
        }

        temporal = temporal->getNext();
    }

    painter.end();
    qDebug() << "Creación de pdf completo";
    QString cmd("xdg-open ");
    cmd.append(filepath);

    system(cmd.toLatin1().data());
}

/**
 * @brief DialogPresentacion::getX
 * @param Indice del 0 - 12
 * @return Coordenada x
 */
int DialogPresentacion::getX(int i)
{
    return 100 * i;
}

/**
 * @brief DialogPresentacion::getY
 * @param Indice del 0 - 6
 * @return Coordenada y
 */
int DialogPresentacion::getY(int j)
{
    return 50 * j;
}

/***********************************************************************************
 * CAPTURAR TEXTO
 **********************************************************************************/
void DialogPresentacion::on_edtTitulo_returnPressed()
{
    currentNode->getData()->setTitulo(ui->edtTitulo->text());
}

void DialogPresentacion::on_edtContenido_textChanged()
{
    currentNode->getData()->setContenido(ui->edtContenido->toPlainText());
}

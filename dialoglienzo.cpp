#include "dialoglienzo.h"
#include "ui_dialoglienzo.h"

DialogLienzo::DialogLienzo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLienzo)
{
    ui->setupUi(this);
    arreglo = NULL;
    filename = "";

    commandIn(false);
    setDimension(true);

    connectClient();
}

DialogLienzo::~DialogLienzo()
{
    delete ui;
    delete arreglo;
}

/***********************************************************************************
 * SETEAR PRIMERA INFORMACION
 **********************************************************************************/
void DialogLienzo::commandIn(bool mostrar)
{
    ui->lblComando->setVisible(mostrar);
    ui->edtComando->setVisible(mostrar);
    ui->btnComando->setVisible(mostrar);
}

void DialogLienzo::setDimension(bool mostrar)
{
    ui->edtX->setEnabled(mostrar);
    ui->edtY->setEnabled(mostrar);
    ui->btnCrear->setVisible(mostrar);
}

void DialogLienzo::setInfo(QString _filename, QString _permiso)
{
    filename = _filename;
    permiso = _permiso;

    if (!filename.isEmpty())
        producer("INFOFILE^" + filename);

    setDimension(false);
}

/***********************************************************************************
 * MANEJO DE CONEXION CLIENTE
 **********************************************************************************/
void DialogLienzo::connectClient()
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

void DialogLienzo::consumer()
{
    QByteArray buffer;
    buffer.resize(tcpCliente->bytesAvailable());
    tcpCliente->read(buffer.data(), buffer.size());
    interpreter(QString (buffer));
}

void DialogLienzo::producer(QString value)
{
    tcpCliente->write(
                value.toLatin1().data(),
                value.size()
                );
}

void DialogLienzo::interpreter(QString mensaje)
{
    QStringList splMensaje = mensaje.split("^");
    qDebug() << "LIENZO " << splMensaje;

    if (mensaje.startsWith("INFOFILE"))
        actionInfoFile(splMensaje);
}

void DialogLienzo::actionInfoFile(QStringList value)
{
    if (value.size() > 1)
    {
        jsd = QJsonDocument::fromJson(value[1].toUtf8());
        loadFile();
    }
}

/***********************************************************************************
 * MANEJO DE ARCHIVO JSON
 **********************************************************************************/
void DialogLienzo::loadFile()
{
    if (jsd.isEmpty())
    {
        qDebug() << "Archivo JSON está vacío";
        return;
    }

    int *n = new int[2]; // LIMITES INFERIORES
    int *m = new int[2]; // LIMITES SUPERIORES

    QJsonObject jso = jsd.object();

    n[0] = jso["lienzo_fila_inf"].toInt();
    m[0] = jso["lienzo_fila_sup"].toInt();
    n[1] = jso["lienzo_columna_inf"].toInt();
    m[1] = jso["lienzo_columna_sup"].toInt();

    arreglo = new Arreglo(n, m);

    QJsonArray jsa = jso["lienzo_bloques"].toArray();

    for (int i = 0; i < jsa.count(); i++)
    {
        QString color;
        int from_i, from_j = 0;
        int to_i, to_j = 0;

        QJsonObject jso_bloque = jsa.at(i).toObject();

        color = jso_bloque["color"].toString();

        if (jso_bloque["fila"].isUndefined() || jso_bloque["fila"].isNull())
        {
            from_i = jso_bloque["fila_inicial"].toInt();
            from_j = jso_bloque["columna_inicial"].toInt();

            to_i = jso_bloque["fila_final"].toInt();
            to_j = jso_bloque["columna_final"].toInt();

            paint(color, from_i, from_j, to_i, to_j);
        }
        else
        {
            from_i = jso_bloque["fila"].toInt();
            from_j = jso_bloque["columna"].toInt();

            paint(color, from_i, from_j);
        }
    }

    commandIn(true);
}

QString DialogLienzo::createJSON()
{
    int iMin = arreglo->getIMin();
    int jMin = arreglo->getJMin();
    int iMax = arreglo->getIMax();
    int jMax = arreglo->getJMax();

    QJsonObject rootJSO;
    rootJSO.insert("lienzo_fila_inf", iMin);
    rootJSO.insert("lienzo_fila_sup", iMax);
    rootJSO.insert("lienzo_columna_inf", jMin);
    rootJSO.insert("lienzo_columna_sup", jMax);

    QJsonArray jsa;
    for (int i = iMin; i <= iMax; i++)
    {
        for (int j = jMin; j <= jMax; j++)
        {
            QJsonObject jso;
            int indice[2];
            QString color;
            indice[0] = i;
            indice[1] = j;

            color = arreglo->getDato(indice);
            if (color.isEmpty())
                continue;

            jso.insert("color", QJsonValue::fromVariant(color));
            jso.insert("fila", QJsonValue::fromVariant(i));
            jso.insert("columna", QJsonValue::fromVariant(j));

            jsa.push_back(jso);
        }
    }

    rootJSO.insert("lienzo_bloques", jsa);
    jsd.setObject(rootJSO);

    QString strJSON(jsd.toJson());

    return strJSON;
}

/***********************************************************************************
 * PINTAR EN MATRIZ
 **********************************************************************************/
void DialogLienzo::paint(QString color, int i, int j)
{
    qDebug() << "i = " << i << " j = " << j;
    int *pos = new int[2];

    pos[0] = i;
    pos[1] = j;

    arreglo->setDato(color, pos);
}

void DialogLienzo::paint(QString color, int from_i, int from_j, int to_i, int to_j)
{
    qDebug() << "i = " << from_i << " - " << to_i << " j = " << from_j << " - " << to_j;
    for (int i = from_i; i <= to_i; i++)
    {
        for (int j = from_j; j <= to_j; j++)
        {
            int *pos = new int[2];
            pos[0] = i;
            pos[1] = j;

            arreglo->setDato(color, pos);
        }
    }
}

/***********************************************************************************
 * ACCIONES DE BOTONES
 **********************************************************************************/
void DialogLienzo::on_btnComando_clicked()
{
    QString comando = ui->edtComando->text();
    if (comando.isEmpty())
        return;

    QStringList cmdParser = comando.split(",");
    qDebug() << cmdParser;
    QString color;
    int x0 = 0;
    int y0 = 0;

    color = cmdParser[0];
    x0 = cmdParser[1].toInt();
    y0 = cmdParser[2].toInt();

    if (cmdParser.count() > 3)
    {
        int x1;
        int y1;

        x1 = cmdParser[3].toInt();
        y1 = cmdParser[4].toInt();
        paint(color, x0, y0, x1, y1);
    }
    else
        paint(color, x0, y0);

    on_btnGenerarPNG_clicked();
}

void DialogLienzo::on_btnCrear_clicked()
{
    int *n = new int[2]; // LIMITES INFERIORES
    int *m = new int[2]; // LIMITES SUPERIORES

    QString x, y;
    x = ui->edtX->text();
    y = ui->edtY->text();

    QStringList split = x.split(",");
    n[0] = split[0].toInt();
    m[0] = split[1].toInt();

    split.clear();
    split = y.split(",");
    n[1] = split[0].toInt();
    m[1] = split[1].toInt();

    arreglo = new Arreglo(n, m);

    setDimension(false);
    commandIn(true);
}

void DialogLienzo::on_btnGuardar_clicked()
{
    QString tipo("lienzo");
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

void DialogLienzo::on_btnEliminar_clicked()
{
}

void DialogLienzo::on_btnGenerarPNG_clicked()
{
    QString texto("digraph lienzo {\n");
    texto.append("node [shape = plaintext]\n");
    texto.append("struct1 [label = \n");
    texto.append("<<TABLE BORDER = \"1\" CELLBORDER = \"0\" CELLSPACING = \"0\">\n");

    texto.append(arreglo->graph());

    texto.append("</TABLE>>\n");
    texto.append("]\n}");

    QFile file("lienzo.dot");
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        out << texto;
        flush(out);
        file.close();
        qDebug() << "Imagen PNG creado";
    }
    else
        qDebug() << "No se pudo crear la imagen png";

    system("dot -Tpng lienzo.dot -o lienzo.png");
    QImage image;
    image.load("lienzo.png");
    ui->lblImagen->setPixmap(QPixmap::fromImage(image).scaled(ui->lblImagen->width(), ui->lblImagen->height(), Qt::KeepAspectRatio));
}

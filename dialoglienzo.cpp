#include "dialoglienzo.h"
#include "ui_dialoglienzo.h"

DialogLienzo::DialogLienzo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLienzo)
{
    ui->setupUi(this);
    arreglo = NULL;
    filename = "";

    conectar();
}

DialogLienzo::~DialogLienzo()
{
    delete ui;
}

void DialogLienzo::setInfo(QString _filename, QString _permiso)
{
    filename = _filename;
    if (!filename.isEmpty())
    {
        producer("INFODOC^" + filename);
    }
}

void DialogLienzo::conectar()
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

    if (mensaje.startsWith("INFODOC"))
    {
        if (splMensaje.size() > 1)
        {
            jsd = QJsonDocument::fromJson(splMensaje[1].toUtf8());
            cargarMatriz();
        }
    }
}

void DialogLienzo::cargarMatriz()
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

            pintar(color, from_i, from_j, to_i, to_j);
        }
        else
        {
            from_i = jso_bloque["fila"].toInt();
            from_j = jso_bloque["columna"].toInt();

            pintar(color, from_i, from_j);
        }

    }
}

void DialogLienzo::pintar(QString color, int i, int j)
{
    int *pos = new int[2];

    pos[0] = i;
    pos[1] = j;

    arreglo->setDato(color, pos);
}

void DialogLienzo::pintar(QString color, int from_i, int from_j, int to_i, int to_j)
{
    for (int x = from_i; x < to_i; x++)
    {
        for (int y = from_j; y < to_j; y++)
        {
            int *pos = new int[2];
            pos[0] = x;
            pos[1] = y;

            arreglo->setDato(color, pos);
        }
    }
}

void DialogLienzo::on_btnComando_clicked()
{

}

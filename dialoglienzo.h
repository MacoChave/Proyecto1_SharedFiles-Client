#ifndef DIALOGLIENZO_H
#define DIALOGLIENZO_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QImage>
#include <QDate>
#include <QInputDialog>
#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "IrregularMatrix/arreglo.h"

namespace Ui {
class DialogLienzo;
}

class DialogLienzo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLienzo(QWidget *parent = 0);
    ~DialogLienzo();

    void setInfo(QString _filename, QString _permiso);

private slots:
    void consumer();

    void producer(QString value);

    void on_btnComando_clicked();

    void on_btnGenerarPNG_clicked();

    void on_btnCrear_clicked();

    void on_btnGuardar_clicked();

    void on_btnEliminar_clicked();

private:
    Ui::DialogLienzo *ui;
    QString filename;
    QString permiso;
    Arreglo *arreglo;

    QJsonDocument jsd;

    QTcpSocket *tcpCliente;

    void connectClient();
    void interpreter(QString mensaje);
    void actionInfoFile(QStringList value);

    void loadFile();
    void paint(QString color, int i, int j);
    void paint(QString color, int from_i, int from_j, int to_i, int to_j);
    void commandIn(bool mostrar);
    void setDimension(bool mostrar);
    QString createJSON();
};

#endif // DIALOGLIENZO_H

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
    Arreglo *arreglo;

    QJsonDocument jsd;

    QTcpSocket *tcpCliente;

    void conectar();
    void interpreter(QString mensaje);
    void cargarMatriz();
    void pintar(QString color, int i, int j);
    void pintar(QString color, int from_i, int from_j, int to_i, int to_j);
    void comandoIn(bool mostrar);
    void seteartDimension(bool mostrar);
};

#endif // DIALOGLIENZO_H

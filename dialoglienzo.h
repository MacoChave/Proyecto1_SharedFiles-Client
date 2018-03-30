#ifndef DIALOGLIENZO_H
#define DIALOGLIENZO_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

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
};

#endif // DIALOGLIENZO_H

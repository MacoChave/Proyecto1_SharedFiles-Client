#ifndef DIALOGDOCUMENT_H
#define DIALOGDOCUMENT_H

#include <QDialog>
#include <QFileDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "GenericTree/generictree.h"
#include "GenericTree/tadgenerictree.h"

namespace Ui {
class DialogDocument;
}

class DialogDocument : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDocument(QWidget *parent = 0);
    ~DialogDocument();

    void setTcp(QTcpSocket *value);
    void setInfo(QString _filename, QString _permiso);

private slots:
    void consumer();

    void producer(QString value);

    void on_btnGuardar_clicked();

    void on_btnCancelar_clicked();

    void on_btnNuevo_clicked();

    void on_btnEditar_clicked();

    void on_btnPDF_clicked();

    void on_btnGrafico_clicked();

    void on_btnEliminar_clicked();

    void on_btnGuardarNodo_clicked();

    void on_btnParrafo_clicked();

    void on_btnVineta_clicked();

    void on_btnImagen_clicked();

    void on_btnChoose_clicked();

private:
    Ui::DialogDocument *ui;
    QString dateMod;
    QString filename;
    GenericTree<NodeGenericTree *> *tree;
    int count;

    QJsonDocument jsd;

    QTcpSocket *tcpCliente;
    int opcion;

    void conectar();
    void interpreter(QString mensaje);
    void cargarArbol();
    List<NodeGenericTree *> *obtenerHijos(QJsonArray currentJSA);
};

#endif // DIALOGDOCUMENT_H

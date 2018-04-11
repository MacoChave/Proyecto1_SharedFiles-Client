#ifndef DIALOGDOCUMENT_H
#define DIALOGDOCUMENT_H

#include <QDialog>
#include <QList>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QByteArray>
#include <QPixmap>
#include <QPrinter>
#include <QPainter>

#include <QTcpSocket>
#include <QHostAddress>

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

    void setInfo(QString _filename, QString _permiso);

private slots:
    void consumer();

    void producer(QString value);

    void on_btnGuardar_clicked();

    void on_btnCancelar_clicked();

    void on_btnNuevo_clicked();

    void on_btnPDF_clicked();

    void on_btnGrafico_clicked();

    void on_btnEliminar_clicked();

    void on_btnGuardarNodo_clicked();

    void on_btnParrafo_clicked();

    void on_btnVineta_clicked();

    void on_btnImagen_clicked();

    void on_btnChoose_clicked();

    void on_edtTitulo_returnPressed();

    void on_edtContenido_textChanged();

private:
    Ui::DialogDocument *ui;
    QString filename;
    QString permiso;
    GenericTree<NodeGenericTree *> *tree;
    NodeGenericTree *currentItem;
    int count;

    QTcpSocket *tcpCliente;

    QJsonDocument jsd;

    int opcion;

    void connectClient();
    void interpreter(QString mensaje);
    void actionInfoFile(QStringList value);
    void actionCoderImage(QStringList value);
    void loadTree();
    List<NodeGenericTree *> *loadChildsTree(QJsonArray currentJSA);
    QString createJSON();
    QJsonArray getChildsTree(List<NodeGenericTree *> *currentList);
    void setData();
    void updateCurrentData();
    QString coderImage(QString value);
    QPixmap decoderImage(QString value);
    void childPDF(QPainter &painter, NodeGenericTree *current, int &y, int level);
    int getX(int i);
    int getY(int j);
};

#endif // DIALOGDOCUMENT_H

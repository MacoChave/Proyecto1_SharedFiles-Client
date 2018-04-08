#ifndef DIALOGPRESENTACION_H
#define DIALOGPRESENTACION_H

#include <QDialog>
#include <QFile>
#include <QInputDialog>
#include <QTextStream>
#include <QFileDialog>
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

#include "List/list.h"
#include "DoubleList/tadlist.h"

namespace Ui {
class DialogPresentacion;
}

class DialogPresentacion : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPresentacion(QWidget *parent = 0);
    ~DialogPresentacion();

    void setInfo(QString _filename, QString _permiso);

    void producer(QString value);

private slots:
    void consumer();

    void on_btnNuevo_clicked();

    void on_btnEliminar_clicked();

    void on_btnTitulo_clicked();

    void on_btnCompleta_clicked();

    void on_btnDoble_clicked();

    void on_btnImagen1_clicked();

    void on_btnImagen2_clicked();

    void on_btnPreview_clicked();

    void on_btnNext_clicked();

    void on_btnGuardar_clicked();

    void on_btnCancelar_clicked();

    void on_btnGrafico_clicked();

    void on_btnPDF_clicked();

    void on_edtTitulo_returnPressed();

    void on_edtContenido_textChanged();

private:
    Ui::DialogPresentacion *ui;
    QString usuario;
    QString filename;
    QString permiso;
    List<TADList *> *lista;
    Node<TADList *> *currentNode;

    QTcpSocket *tcpCliente;

    QJsonDocument jsd;

    void connectClient();
    void interpreter(QString message);
    void actionInfoFile(QStringList value);
    void actionCoderImage(QStringList value);
    bool loadList();
    QString createJSON();
    void setData();
    void updateCurrentData();
    void coderImage(QString value);
    void decoderImage(QString value);
    int getX(int i);
    int getY(int i);
};

#endif // DIALOGPRESENTACION_H

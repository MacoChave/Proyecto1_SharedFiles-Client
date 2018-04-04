#ifndef DIALOGPRESENTACION_H
#define DIALOGPRESENTACION_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

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

    void on_btnGuardar_clicked();

    void on_btnImagen1_clicked();

    void on_btnImagen2_clicked();

    void on_btnPreview_clicked();

    void on_btnNext_clicked();

    void on_btnGuardar_2_clicked();

    void on_btnCancelar_clicked();

    void on_btnGrafico_clicked();

    void on_btnPDF_clicked();

    void on_btnTitulo_clicked();

    void on_btnCompleta_clicked();

    void on_btnDoble_clicked();

private:
    Ui::DialogPresentacion *ui;
    QString usuario;
    QString filename;
    QString permiso;
    QTcpSocket *tcpCliente;
    List<TADList *> *lista;

    QJsonDocument jsd;

    void conectar();
    void interpreter(QString mensaje);
    void actionInfoFile(QStringList value);
    void cargarLista();
};

#endif // DIALOGPRESENTACION_H

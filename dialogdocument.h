#ifndef DIALOGDOCUMENT_H
#define DIALOGDOCUMENT_H

#include <QDialog>
#include <QFileDialog>
#include <QTcpSocket>
#include <QList>

#include "GenericTree/generictree.h"

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

private slots:
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
    QString filename;
    QTcpSocket *tcpCliente;
    int opcion;
};

#endif // DIALOGDOCUMENT_H

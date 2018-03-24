#include "dialogdocument.h"
#include "ui_dialogdocument.h"

DialogDocument::DialogDocument(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDocument)
{
    ui->setupUi(this);
    filename = "";
    opcion = 0;
}

DialogDocument::~DialogDocument()
{
    delete ui;
}

void DialogDocument::setTcp(QTcpSocket *value)
{
    tcpCliente = value;
}

void DialogDocument::on_btnGuardar_clicked()
{

}

void DialogDocument::on_btnCancelar_clicked()
{

}

void DialogDocument::on_btnNuevo_clicked()
{

}

void DialogDocument::on_btnEditar_clicked()
{

}

void DialogDocument::on_btnPDF_clicked()
{

}

void DialogDocument::on_btnGrafico_clicked()
{

}

void DialogDocument::on_btnEliminar_clicked()
{

}

void DialogDocument::on_btnGuardarNodo_clicked()
{
    NodeGenericTree *node = NULL;
    TADGenericTree *tad = new TADGenericTree;
    tad->setTitulo(ui->edtTitulo->text());

    switch (opcion) {
    case 1: // PARRAFO
    {
        tad->setContenido(ui->edtContenido->document()->toPlainText());
        node = new NodeGenericTree(tad);
        break;
    }
    case 2: // VIÑETAS
    {
        tad->setContenido("...");
        node = new NodeGenericTree();
        node->setData(tad);
        qDebug() << tad->getContenido();
        break;
    }
    case 3: // IMAGEN
    {
        tad->setContenido(ui->edtImagen->text());
        node = new NodeGenericTree(tad);
    }
    default:
        delete tad;
        tad = NULL;
        return;
        break;
    }
}

void DialogDocument::on_btnParrafo_clicked()
{
    opcion = 1;

    ui->lblTitulo->setVisible(true);
    ui->lblTitulo->setText("Titulo (Opcional)");
    ui->edtImagen->setVisible(true);

    ui->lblContenido->setVisible(true);
    ui->lblContenido->setText("Contenido (Opcional)");
    ui->edtContenido->setVisible(true);

    ui->lblImagen->setVisible(false);
    ui->edtImagen->setVisible(false);
    ui->btnChoose->setVisible(false);
}

void DialogDocument::on_btnVineta_clicked()
{
    opcion = 2;

    ui->lblTitulo->setVisible(true);
    ui->lblTitulo->setText("Titulo (Descripción de la lista de elementos)");
    ui->edtImagen->setVisible(true);

    ui->lblContenido->setVisible(true);
    ui->lblContenido->setText("Ítems (Un elemento por cada línea)");
    ui->edtContenido->setVisible(true);

    ui->lblImagen->setVisible(false);
    ui->edtImagen->setVisible(false);
    ui->btnChoose->setVisible(false);
}

void DialogDocument::on_btnImagen_clicked()
{
    opcion = 3;

    ui->lblTitulo->setVisible(true);
    ui->lblTitulo->setText("Titulo (Pie de la imagen)");
    ui->edtImagen->setVisible(true);

    ui->lblContenido->setVisible(false);
    ui->edtContenido->setVisible(false);

    ui->lblImagen->setVisible(true);
    ui->lblImagen->setText("Imagen");
    ui->edtImagen->setVisible(true);
    ui->btnChoose->setVisible(true);
}

void DialogDocument::on_btnChoose_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                "Seleccion de imagen",
                "/home/marco/Escritorio/",
                "Archivo de imagen (*.jpg) | Archivo de imagen (*.png)"
                );
    ui->edtImagen->setText(filename);
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QList>

#include "dialoglogin.h"
#include "dialogdocument.h"
#include "dialogpresentacion.h"
#include "dialoglienzo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btnActualizar_clicked();

    void on_btnLogin_clicked();

    void on_btnLogout_clicked();

    void on_btnEliminar_clicked();

    void on_btnVer_clicked();

    void on_btnNuevo_clicked();

    void on_btnCompartir_clicked();

    void on_btnRestringir_clicked();

    void on_btnOpcion_clicked();

    void on_btnOcultar_clicked();

    void on_btnDocumento_clicked();

    void on_btnPresentacion_clicked();

    void on_btnLienzo_clicked();

    void on_btnReloadCompartidos_clicked();

public slots:
    void consumer();

    void producer(QString value);

private:
    Ui::MainWindow *ui;
    QString usuario;
    QString pathTemporal;
    QTcpSocket *tcpCliente;

    void connectClient();
    void disconnectClient();
    void interpreter(QString mensaje);

    void actionSesion(QStringList value);
    void actionListFiles(QStringList value);
    void actionInfoFile(QStringList value);
    void actionCreateFile(QStringList value);
    void actionUpdateFile(QStringList value);
    void actionDeleteFile(QStringList value);
    void actionListUser(QStringList value);
    void actionSharedUser(QStringList value);

    void loadInformation();
    void loadComboBox();
    void clearInformation();
    void cleanTable();
    void cleanComboBox();
};

#endif // MAINWINDOW_H

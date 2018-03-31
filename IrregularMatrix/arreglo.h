#ifndef ARREGLO_H
#define ARREGLO_H
#include <QString>
#include <QDebug>

class Arreglo
{
    int *n; // LIMITES INFERIORES
    int *m; // LIMITES SUPERIORES
    QString *alfa; // DIRECCION DEL PRIMER ELEMENTO (ARREGLO LINEALIZADO)
public:
    Arreglo(int n[], int m[]);
    ~Arreglo();

    void setDato(QString value, int *indice);
    QString getDato(int *indice);
    int getLoc(int *indice);
    QString graph();
};

#endif // ARREGLO_H

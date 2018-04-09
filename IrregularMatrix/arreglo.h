#ifndef ARREGLO_H
#define ARREGLO_H
#include <QString>
#include <QDebug>

class Arreglo
{
    int *n; // LIMITES INFERIORES [i_min, j_min]
    int *m; // LIMITES SUPERIORES [i_max, j_max]
    QString *alfa; // DIRECCION DEL PRIMER ELEMENTO (ARREGLO LINEALIZADO) [jk, ghj, hjk]
public:
    Arreglo(int n[], int m[]);
    ~Arreglo();

    void setDato(QString value, int *indice);
    QString getDato(int *indice);
    int getIMin();
    int getIMax();
    int getJMin();
    int getJMax();
    int getLoc(int *indice);
    int getSize();
    QString graph();
};

#endif // ARREGLO_H

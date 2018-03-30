#ifndef ARREGLO_H
#define ARREGLO_H
#include <QString>

class Arreglo
{
    int *n; // LIMITES INFERIORES
    int *m; // LIMITES SUPERIORES
    QString *alfa; // DIRECCION DEL PRIMER ELEMENTO (ARREGLO LINEALIZADO)
public:
    Arreglo(int n[], int m[]);
    ~Arreglo();

    void setDato(QString value, int *indice);
    QString getDato(int *i);
    int getLoc(int *indice);
};

#endif // ARREGLO_H

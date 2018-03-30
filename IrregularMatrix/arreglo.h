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

    QString &operator [] (int *i);
};

#endif // ARREGLO_H

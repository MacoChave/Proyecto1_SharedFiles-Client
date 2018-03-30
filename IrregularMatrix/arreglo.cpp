#include "arreglo.h"

Arreglo::Arreglo(int n[], int m[])
{
    this->n = new int[2];
    this->m = new int[2];

    int size = 1;
    for (int i = 0; i < 2; i++)
    {
        this->n[i] = n[i];
        this->m[i] = m[i];

        size *= this->m[i] - this->n[i];
    }

    alfa = new QString[size];
}

Arreglo::~Arreglo()
{
    delete n;
    n = 0;
    delete m;
    m = 0;
    delete alfa;
    alfa = 0;
}

void Arreglo::setDato(QString value, int *indice)
{
//    int loc = getLoc(indice);
}

QString Arreglo::getDato(int *i)
{
//    int loc = getLoc(indice);

    return "";
}

int Arreglo::getLoc(int *indice)
{
    int dim_j = m[1] - n[1];
    int i = indice[0] - n[0];
    int j = indice[1] - n[1];

    return dim_j * i + j;
}

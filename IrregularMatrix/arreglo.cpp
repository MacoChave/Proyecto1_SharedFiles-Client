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

QString &Arreglo::operator [](int *i)
{
    int loc = 0;

    /*
     * LOC(i,j) = (i - i_inf) * J + (j - j_inf)
     */
    int dim_j = 0;
    dim_j = m[1] - n[1];

    loc = (i[0] - n[0])*dim_j + (i[1] - n[1]);

    return alfa[loc];
}

#include "arreglo.h"

Arreglo::Arreglo(int n[], int m[])
{
    this->n = new int[2];
    this->m = new int[2];

    this->n[0] = n[0];
    this->n[1] = n[1];
    this->m[0] = m[0];
    this->m[1] = m[1];

    int size = 0;
    int dim_i = (this->m[0] - this->n[0]) + 1;
    int dim_j = (this->m[1] - this->n[1]) + 1;
    qDebug() << "Arreglo[" << dim_i << "][" << dim_j << "]";

    size = dim_i * dim_j;
    qDebug() << "Arreglo[" << size << "]";

    alfa = new QString[size];
}

Arreglo::~Arreglo()
{
    delete n;
    n = 0;
    delete m;
    m = 0;
    alfa = 0;
}

void Arreglo::setDato(QString value, int *indice)
{
    int loc = getLoc(indice);
    alfa[loc] = value;
}

QString Arreglo::getDato(int *indice)
{
    int loc = getLoc(indice);

    return alfa[loc];
}

int Arreglo::getLoc(int *indice)
{
    int dim_j = (m[1] - n[1]) + 1;
    int i = indice[0] - n[0];
    int j = indice[1] - n[1];

    int loc = (i * dim_j) + j;
    return loc;
}

QString Arreglo::graph()
{
    QString texto;

    for (int i = n[0]; i <= m[0]; i++)
    {
//        <TR>
        texto.append("<TR>\n");
        for (int j = n[1]; j <= m[1]; j++)
        {
//            <TD bgcolor = "#1d1d1d">
            texto.append("<TD bgcolor = \"");

            int *indice = new int[2];
            indice[0] = i;
            indice[1] = j;
            QString value = getDato(indice);

            if (!value.isEmpty())
            {
                texto.append(value);
            }
            else
            {
                texto.append("#FFFFFF");
            }

            texto.append("\">");
//            </TD>
            texto.append("</TD>\n");
        }
//        </TR>
        texto.append("</TR>\n");
    }

    return texto;
}

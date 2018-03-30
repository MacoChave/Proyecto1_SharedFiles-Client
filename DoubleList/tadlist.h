#ifndef TADLIST_H
#define TADLIST_H
#include <QString>

class TADList
{
    int layout;
    QString titulo;
    QString contenido;
    QString imagen1;
    QString imagen2;
public:
    TADList();
    ~TADList();

    enum
    {
        TITULO, COMPLETO, DOBLE
    };

    void setLayout(int value);
    int getlayout();
    void setTitulo(QString value);
    QString getTitulo();
    void setContenido(QString value);
    QString getContenido();
    void setImagen1(QString value);
    QString getImagen1();
    void setImagen2(QString value);
    QString getImagen2();

    int compare(TADList *value);
    QString toString();
    QString getNodeName();
    QString createNode();
    QString pointNode(TADList *value);
};

#endif // TADLIST_H

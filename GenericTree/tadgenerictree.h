#ifndef TADGENERICTREE_H
#define TADGENERICTREE_H
#include <QString>

class TADGenericTree
{
    int id;
    QString titulo;
    QString contenido;
    int tipo;

public:
    TADGenericTree();
    TADGenericTree(int _id, QString _titulo, QString _contenido);
    TADGenericTree(int _id, QString _contenido);
    ~TADGenericTree();

    enum
    {
        PARRAFO, VINETAS, IMAGEN
    };

    void setId(int value);
    int getId();
    void setTitulo(QString value);
    QString getTitulo();
    void setContenido(QString value);
    QString getContenido();
    void setTipo(int _tipo);
    int getTipo();
    int compare(TADGenericTree *value);
    QString toString();
    QString getNodeName();
    QString createNode();
    QString pointNode(TADGenericTree *next);
};

#endif // TADGENERICTREE_H

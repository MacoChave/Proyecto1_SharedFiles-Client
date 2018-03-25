#include "tadgenerictree.h"

TADGenericTree::TADGenericTree()
{
    id = 0;
    titulo = "";
    contenido = "";
    tipo = -1;
}

TADGenericTree::TADGenericTree(int _id, QString _titulo, QString _contenido)
{
    id = _id;
    titulo = _titulo;
    contenido = _contenido;
    tipo = -1;
}

TADGenericTree::TADGenericTree(int _id, QString _contenido)
{
    id = _id;
    titulo = "";
    contenido = _contenido;
    tipo = -1;
}

TADGenericTree::~TADGenericTree()
{
    id = 0;
    titulo.clear();
    contenido.clear();
    tipo = -1;
}

void TADGenericTree::setId(int value)
{
    id = value;
}

int TADGenericTree::getId()
{
    return id;
}

void TADGenericTree::setTitulo(QString value)
{
    titulo = value;
}

QString TADGenericTree::getTitulo()
{
    return titulo;
}

void TADGenericTree::setContenido(QString value)
{
    contenido = value;
}

QString TADGenericTree::getContenido()
{
    return contenido;
}

void TADGenericTree::setTipo(int _tipo)
{
    tipo = _tipo;
}

int TADGenericTree::getTipo()
{
    return tipo;
}

int TADGenericTree::compare(TADGenericTree *value)
{
    if (id > value->getId())
        return 1;
    if (id < value->getId())
        return -1;

    return 0;
}

QString TADGenericTree::toString()
{
    QString text;
    text.append(QString::number(id));
    text.append("\\n");
    text.append(titulo);
    switch (tipo) {
    case 0:
        text.append("\\nPARRAFO");
        break;
    case 1:
        text.append("\\nVINETAS");
        break;
    case 2:
        text.append("\\nIMAGEN");
        break;
    default:
        break;
    }

    return text;
}

QString TADGenericTree::getNodeName()
{
    QString node("node");
    node.append(QString::number(id));

    return node;
}

QString TADGenericTree::createNode()
{
    QString node("\n");
    node.append(getNodeName());
    node.append(" [label = \"");
    node.append(toString());
    node.append("\"];\n");

    return node;
}

QString TADGenericTree::pointNode(TADGenericTree *next)
{
    QString node("\n");
    if (next != NULL)
    {
        node.append(getNodeName());
        node.append(" -> ");
        node.append(next->getNodeName());
        node.append(";\n");
    }

    return node;
}

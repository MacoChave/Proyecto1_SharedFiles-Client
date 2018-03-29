#include "tadlist.h"

TADList::TADList()
{
    layout = 0;
    titulo = "";
    contenido = "";
    imagen1 = "";
    imagen2 = "";
}

TADList::~TADList()
{
    layout = 0;
    titulo.clear();
    contenido.clear();
    imagen1.clear();
    imagen2.clear();
}

void TADList::setLayout(int value)
{
    layout = value;
}

int TADList::getlayout()
{
    return layout;
}

void TADList::setTitulo(QString value)
{
    titulo = value;
}

QString TADList::getTitulo()
{
    return titulo;
}

void TADList::setContenido(QString value)
{
    contenido = value;
}

QString TADList::getContenido()
{
    return contenido;
}

void TADList::setImagen1(QString value)
{
    imagen1 = value;
}

QString TADList::getImagen1()
{
    return imagen1;
}

void TADList::setImagen2(QString value)
{
    imagen2 = value;
}

QString TADList::getImagen2()
{
    return imagen2;
}

int TADList::compare(TADList *value)
{
    return titulo.compare(value->getTitulo());
}

QString TADList::toString()
{
    QString string;
    string.append(titulo);
    string.append("\\n");
    switch (layout) {
    case TITULO:
        string.append("Titulo");
        break;
    case COMPLETO:
        string.append("Completo");
        break;
    case DOBLE:
        string.append("Doble");
        break;
    }

    return string;
}

QString TADList::getNodeName()
{
    QString name("node");
    name.append(titulo);

    return name;
}

QString TADList::createNode()
{
    QString node;
    node.append(getNodeName());
    node.append(" label [\"");
    node.append(toString());
    node.append("\"];\n");

    return node;
}

QString TADList::pointNode(TADList *value)
{
    QString node("\n");
    if (next != NULL)
    {
        node.append(getNodeName());
        node.append(" -> ");
        node.append(value->getNodeName());
        node.append(";\n");

        node.append(value->getNodeName());
        node.append(" -> ");
        node.append(getNodeName());
        node.append(";\n");
    }

    return node;
}

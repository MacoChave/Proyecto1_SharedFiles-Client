#ifndef GENERICTREE_H
#define GENERICTREE_H
#include"nodegenerictree.h"

template <typename T>
class GenericTree
{
    NodeGenericTree *root;
    int contador;

    QString graph(NodeGenericTree *current);
public:
    GenericTree();
    ~GenericTree();

    void setRoot(NodeGenericTree *value);
    QString graph();
};

#endif // GENERICTREE_H

template <class T>
QString GenericTree<T>::graph(NodeGenericTree *current)
{
    QString text("");
    Node<T> *child = NULL;
    if (current->getChilds() != NULL)
        child = current->getChilds()->first();

    while (child != NULL)
    {
        text.append(child->createNode());

        text.append(current->getNodeName());
        text.append(" -> ");
        text.append(child->getNameNode());

        text.append(graph(child->getData()));
        child = child->getNext();
    }

    return text;
}

template <class T>
GenericTree<T>::GenericTree()
{
    root = NULL;
    contador = 0;
}

template <class T>
GenericTree<T>::~GenericTree()
{
    contador = 0;
    if (root != NULL)
        delete root;
    root = NULL;
}

template <class T>
void GenericTree<T>::setRoot(NodeGenericTree *value)
{
    root = value;
}

template <class T>
QString GenericTree<T>::graph()
{
    QString text;
    text.append("digraph arbolito");
    text.append(" {\n");
    text.append("node [shape = \"box\"]\n");

    if (root != NULL)
        text.append(root->createNode());

    text.append(graph(root));

    text.append("}");

    return text;
}

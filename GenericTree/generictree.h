#ifndef GENERICTREE_H
#define GENERICTREE_H
#include <QDebug>
#include "nodegenerictree.h"

template <typename T>
class GenericTree
{
    NodeGenericTree *root;
    int contador;

    QString graph(NodeGenericTree *current);
    NodeGenericTree *getNode(int value, NodeGenericTree *current);
    void resetIdNode(NodeGenericTree *current);
public:
    GenericTree();
    ~GenericTree();

    bool empty();
    void setRoot(NodeGenericTree *value);
    NodeGenericTree *getRoot();
    NodeGenericTree *getNode(int value);
    void resetIdNode();
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
NodeGenericTree *GenericTree<T>::getNode(int value, NodeGenericTree *current)
{
    NodeGenericTree *result = NULL;

    if (current == NULL)
        return result;

    if (current->getData()->getId() == value)
        return current;

    if (current->getChilds() == NULL)
        return result;

    Node<NodeGenericTree *> *node = current->getChilds()->first();
    while (node != NULL)
    {
        result = getNode(value, node->getData());
        if (result != NULL)
            break;

        node = node->getNext();
    }

    return result;
}

template <class T>
void GenericTree<T>::resetIdNode(NodeGenericTree *current)
{
    if (current == NULL)
        return;

    current->getData()->setId(contador);
    contador++;

    if (current->getChilds() == NULL)
        return;

    Node<NodeGenericTree *> *node = current->getChilds()->first();
    while (node != NULL)
    {
        resetIdNode(node->getData());
        node = node->getNext();
    }
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
bool GenericTree<T>::empty()
{
    return root == NULL;
}

template <class T>
void GenericTree<T>::setRoot(NodeGenericTree *value)
{
    root = value;
}

template <class T>
NodeGenericTree *GenericTree<T>::getRoot()
{
    return root;
}

template <class T>
NodeGenericTree *GenericTree<T>::getNode(int value)
{
    NodeGenericTree *node = getNode(value, root);
    return node;
}

template <class T>
void GenericTree<T>::resetIdNode()
{
    contador = 0;
    resetIdNode(root);
}

template <class T>
QString GenericTree<T>::graph()
{
    QString text;
    if (root != NULL)
    {
        text = root->createNode();
        text.append(graph(root));
    }
    else
        qDebug() << "Arbol es vacío";

    return text;
}

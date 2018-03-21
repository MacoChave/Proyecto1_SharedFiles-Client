#ifndef NODE_H
#define NODE_H
#include <QString>

template <typename T>
class Node
{
    T data;
    Node<T> *next;
    Node<T> *preview;
public:
    Node();
    Node(T value);
    ~Node();

    T getData();
    void setData(T value);
    Node<T> *getNext();
    void setNext(Node<T> *value);
    Node<T> *getPreview();
    void setPreview(Node<T> *value);

    QString getNameNode();
    QString toString();
    /*
    int compare(T value);
     */
    QString createNode();
    QString pointNode();
};

#endif // NODE_H

template <typename T>
Node<T>::Node()
{
    data = NULL;
    next = NULL;
    preview = NULL;
}

template <typename T>
Node<T>::Node(T value)
{
    data = value;
    next = NULL;
    preview = NULL;
}

template <typename T>
Node<T>::~Node()
{
    if (data != NULL)
        delete data;
    data = NULL;
    next = NULL;
    preview = NULL;
}

template <typename T>
T Node<T>::getData()
{
    return data;
}

template <typename T>
void Node<T>::setData(T value)
{
    data = value;
}

template <typename T>
Node<T> *Node<T>::getNext()
{
    return next;
}

template <typename T>
void Node<T>::setNext(Node<T> *value)
{
    next = value;
}

template <typename T>
Node<T> *Node<T>::getPreview()
{
    return preview;
}

template <typename T>
void Node<T>::setPreview(Node<T> *value)
{
    preview = value;
}

template <typename T>
QString Node<T>::getNameNode()
{
    return data->getNodeName();
}

template <typename T>
QString Node<T>::toString()
{
    return data->toString();
}

template <typename T>
QString Node<T>::createNode()
{
    return data->createNode();
}

template <typename T>
QString Node<T>::pointNode()
{
    if (next != NULL)
        return data->pointNode(next->getData());
    else
        return data->pointNode(NULL);
}

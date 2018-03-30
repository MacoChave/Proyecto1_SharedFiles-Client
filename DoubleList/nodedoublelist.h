#ifndef NODEDOUBLELIST_H
#define NODEDOUBLELIST_H
#include <QString>

template <typename T>
class DoubleNode
{
    T data;
    DoubleNode<T> *next;
    DoubleNode<T> *preview;
public:
    DoubleNode();
    DoubleNode(T value);
    ~DoubleNode();

    T getData();
    void setData(T value);
    DoubleNode<T> *getNext();
    void setNext(DoubleNode<T> *value);
    DoubleNode<T> *getPreview();
    void setPreview(DoubleNode<T> *value);

    QString getNameNode();
    QString toString();
    /*
    int compare(T value);
     */
    QString createNode();
    QString pointNode();
};

#endif // NODEDOUBLELIST_H

template <typename T>
DoubleNode<T>::DoubleNode()
{
    data = NULL;
    next = NULL;
    preview = NULL;
}

template <typename T>
DoubleNode<T>::DoubleNode(T value)
{
    data = value;
    next = NULL;
    preview = NULL;
}

template <typename T>
DoubleNode<T>::~DoubleNode()
{
    if (data != NULL)
        delete data;
    data = NULL;
    next = NULL;
    preview = NULL;
}

template <typename T>
T DoubleNode<T>::getData()
{
    return data;
}

template <typename T>
void DoubleNode<T>::setData(T value)
{
    data = value;
}

template <typename T>
DoubleNode<T> *DoubleNode<T>::getNext()
{
    return next;
}

template <typename T>
void DoubleNode<T>::setNext(DoubleNode<T> *value)
{
    next = value;
}

template <typename T>
DoubleNode<T> *DoubleNode<T>::getPreview()
{
    return preview;
}

template <typename T>
void DoubleNode<T>::setPreview(DoubleNode<T> *value)
{
    preview = value;
}

template <typename T>
QString DoubleNode<T>::getNameNode()
{
    return data->getNodeName();
}

template <typename T>
QString DoubleNode<T>::toString()
{
    return data->toString();
}

template <typename T>
QString DoubleNode<T>::createNode()
{
    return data->createNode();
}

template <typename T>
QString DoubleNode<T>::pointNode()
{
    if (next != NULL)
        return data->pointNode(next->getData());
    else
        return data->pointNode(NULL);
}

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
    Node()
    {
        data = NULL;
        next = NULL;
        preview = NULL;
    }
    Node(T value)
    {
        data = value;
        next = NULL;
        preview = NULL;
    }
    ~Node()
    {
        if (data != NULL)
            delete data;
        data = NULL;
        next = NULL;
        preview = NULL;
    }

    T getData()
    {
        return data;
    }
    void setData(T value)
    {
        data = value;
    }
    Node<T> *getNext()
    {
        return next;
    }
    void setNext(Node<T> *value)
    {
        next = value;
    }
    Node<T> *getPreview()
    {
        return preview;
    }
    void setPreview(Node<T> *value)
    {
        preview = value;
    }

    QString getNameNode()
    {
        return data->getNodeName();
    }
    QString toString()
    {
        return data->toString();
    }
    /*
    int compare(T value);
     */
    QString createNode()
    {
        return data->createNode();
    }
    QString pointNode()
    {
        if (next != NULL)
            return data->pointNode(next->getData());
        else
            return data->pointNode(NULL);
    }
};

#endif // NODE_H

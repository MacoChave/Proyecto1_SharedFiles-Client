#ifndef DOUBLELIST_H
#define DOUBLELIST_H
#include "nodedoublelist.h"

template <typename T>
class DoubleList
{
    int count;
    DoubleNode<T> *head;
    DoubleNode<T> *tail;

    DoubleNode<T> *insert(DoubleNode<T> *current, T value);
    DoubleNode<T> *get(DoubleNode<T> *current, T value);
public:
    DoubleList();
    DoubleList(DoubleList<T> *value);
    ~DoubleList();

    /* CAPACIDAD */
    bool isEmpty();
    int size();
    void clear();

    /* MODIFICADORES */
    DoubleNode<T> *push_front(T value);
    DoubleNode<T> *push_back(T value);
    void pop_front();
    void pop_back();
    DoubleNode<T> *insert(T value);
    void erase(T value);
    DoubleNode<T> *removeFirst();
    DoubleNode<T> *removeLast();
    DoubleNode<T> *removeOne(T value);

    /* OPERACIONES */
    DoubleNode<T> *first();
    DoubleNode<T> *last();
    T front();
    T back();
    DoubleNode<T> *get(T value);
    QString graph();
    void sort();
    void reverse();
};

#endif // DOUBLELIST_H

/*
 * CONSTRUCTORES Y DESTRUCTOR
*/
template <typename T>
DoubleNode<T> *DoubleList<T>::insert(DoubleNode<T> *current, T value)
{
    if (current == NULL)
        return NULL;

    int compare = current->getData()->compare(value);

    if (compare == 0)
    {
        delete value;
        value = NULL;

        return current;
    }
    else
    {
        if (compare > 0)
        {
            DoubleNode<T> *temporal = new DoubleNode<T>(value);
            temporal->setNext(current);
            temporal->setPreview(current->getPreview());
            current->getPreview()->setNext(temporal);
            current->setPreview(temporal);
            count++;

            return temporal;
        }
        else
            return insert(current->getNext(), value);
    }
}

template <typename T>
DoubleNode<T> *DoubleList<T>::get(DoubleNode<T> *current, T value)
{
    if (current == NULL)
        return NULL;

    int compare = current->getData()->compare(value);

    if (compare == 0)
        return current;
    else
    {
        if (compare < 0)
            return get(current->getNext(), value);
        else
            return NULL;
    }
}

template <typename T>
DoubleList<T>::DoubleList()
{
    head = NULL;
    tail = NULL;
    count = 0;
}

template <typename T>
DoubleList<T>::DoubleList(DoubleList<T> *value)
{
    head = value->first();
    tail = value->last();
    count = value->size();

    value = NULL;
}

template <typename T>
DoubleList<T>::~DoubleList()
{
    clear();
}

/*
 * CAPACIDAD
*/
template <typename T>
bool DoubleList<T>::isEmpty()
{
    return head == NULL;
}

template <typename T>
int DoubleList<T>::size()
{
    return count;
}

template <typename T>
void DoubleList<T>::clear()
{
    while (!isEmpty())
        pop_front();
}

/*
 * MODIFICADORES
*/
template <typename T>
DoubleNode<T> *DoubleList<T>::push_front(T value)
{
    DoubleNode<T> *node = new DoubleNode<T>(value);

    if (isEmpty())
        head = tail = node;
    else
    {
        node->setNext(head);
        head->setPreview(node);
        head = node;
    }

    count++;

    return node;
}

template <typename T>
DoubleNode<T> *DoubleList<T>::push_back(T value)
{
    DoubleNode<T> *node = new DoubleNode<T>(value);

    if (isEmpty())
        head = tail = node;
    else
    {
        node->setPreview(tail);
        tail->setNext(node);
        tail = node;
    }

    count++;

    return node;
}

template <typename T>
void DoubleList<T>::pop_front()
{
    if (isEmpty())
        return;

    DoubleNode<T> *temporal = head;

    head = temporal->getNext();
    if (size() > 1)
        head->setPreview(NULL);
    else
        tail = head;

    delete temporal;
    temporal = NULL;
    count--;
}

template <typename T>
void DoubleList<T>::pop_back()
{
    if (isEmpty())
        return;

    DoubleNode<T> *temporal = tail;

    tail = temporal->getPreview();
    if (size() > 1)
        tail->setNext(NULL);
    else
        head = tail;

    delete temporal;
    temporal = NULL;
    count--;
}

template <typename T>
DoubleNode<T> *DoubleList<T>::insert(T value)
{
    if (isEmpty())
    {
        DoubleNode<T> *temporal = new DoubleNode<T>(value);
        head = tail = temporal;
        count++;

        return temporal;
    }
    else
    {
        if (head->getData()->compare(value) > 0)
            return push_front(value);
        else if (tail->getData()->compare(value) < 0)
            return push_back(value);
        else
            return insert(head, value);
    }
    return NULL;
}

template <typename T>
void DoubleList<T>::erase(T value)
{
    DoubleNode<T> *temporal = removeOne(value);
    delete temporal;
    temporal = NULL;
}

template <typename T>
DoubleNode<T> *DoubleList<T>::removeFirst()
{
    if (isEmpty())
        return NULL;

    DoubleNode<T> *temporal = head;
    head = temporal->getNext();
    if (size() > 1)
        head->setPreview(NULL);
    else
        tail = head;
    count--;

    return temporal;
}

template <typename T>
DoubleNode<T> *DoubleList<T>::removeLast()
{
    if (isEmpty())
        return NULL;

    DoubleNode<T> *temporal = tail;
    tail = temporal->getPreview();
    if (size() > 1)
        tail->setNext(NULL);
    else
        head = tail;
    count--;

    return temporal;
}

template <typename T>
DoubleNode<T> *DoubleList<T>::removeOne(T value)
{
    if (head->getData()->compare(value) == 0)
        return removeFirst();
    if (tail->getData()->compare(value) == 0)
        return removeLast();

    if (head->getData()->compare(value) < 0
            && tail->getData()->compare(value) > 0)
    {
        DoubleNode<T> *temporal = get(head->getNext(), value);

        if (temporal->getNext() != NULL)
            temporal->getNext()->setPreview(temporal->getPreview());
        if (temporal->getPreview() != NULL)
            temporal->getPreview()->setNext(temporal->getNext());

        temporal->setNext(NULL);
        temporal->setPreview(NULL);
        count--;

        return temporal;
    }
    else
        return NULL;
}

/*
 * OPERACIONES
*/
template <typename T>
DoubleNode<T> *DoubleList<T>::first()
{
    return head;
}

template <typename T>
DoubleNode<T> *DoubleList<T>::last()
{
    return tail;
}

template <typename T>
T DoubleList<T>::front()
{
    return head->getData();
}

template <typename T>
T DoubleList<T>::back()
{
    return tail->getData();
}

template <typename T>
DoubleNode<T> *DoubleList<T>::get(T value)
{
    if (isEmpty())
        return NULL;

    if (head->getData()->compare(value) == 0)
        return head;
    if (tail->getData()->compare(value) == 0)
        return tail;

    if (head->getData()->compare(value) < 0
            && tail->getData()->compare(value) > 0)
        return get(head->getNext(), value);
    else
        return NULL;
}

template <typename T>
QString DoubleList<T>::graph()
{
    QString listGraph;

    DoubleNode<T> *temporal = head;
    while (temporal != NULL)
    {
        listGraph.append(temporal->createNode());
        listGraph.append(temporal->pointNode());

        temporal = temporal->getNext();
    }

    return listGraph;
}

/*
    1. for (i=1; i<TAM; i++)
    2.      temp = lista[i];
    3.      j = i - 1;
    4.      while ( (lista[j] > temp) && (j >= 0) )
    5.           lista[j+1] = lista[j];
    6.           j--;
    7.      lista[j+1] = temp;

    5 9 1 8 2 7 3 6 4
*/
/* ORDENAMIENTO INSERCION */
template <typename T>
void DoubleList<T>::sort()
{
    DoubleNode<T> *i = NULL;

    for (i = head->getNext(); i != NULL; i = i->getNext())
    {
        DoubleNode<T> *j = i->getPreview();
        while (j != NULL && j->getData()->compare(i->getData()) > 0)
        {

        }
    }
}

/* ORDENAMIENTO ? */
template <typename T>
void DoubleList<T>::reverse()
{
}

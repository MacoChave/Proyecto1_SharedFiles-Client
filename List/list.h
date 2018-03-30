#ifndef LIST_H
#define LIST_H
#include "node.h"

template <typename T>
class List
{
    int count;
    Node<T> *head;
    Node<T> *tail;

    Node<T> *insert(Node<T> *current, T value);
    Node<T> *get(Node<T> *current, T value);
public:
    List();
    List(List<T> *value);
    ~List();

    /* CAPACIDAD */
    bool isEmpty();
    int size();
    void clear();

    /* MODIFICADORES */
    Node<T> *push_front(T value);
    Node<T> *push_back(T value);
    void pop_front();
    void pop_back();
    Node<T> *insert(T value);
    void erase(T value);
    Node<T> *removeFirst();
    Node<T> *removeLast();
    Node<T> *removeOne(T value);

    /* OPERACIONES */
    Node<T> *first();
    Node<T> *last();
    T front();
    T back();
    Node<T> *get(T value);
    QString graph();
    void sort();
    void reverse();
};

#endif // LIST_H

/*
 * CONSTRUCTORES Y DESTRUCTOR
*/
template <typename T>
Node<T> *List<T>::insert(Node<T> *current, T value)
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
            Node<T> *temporal = new Node<T>(value);
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
Node<T> *List<T>::get(Node<T> *current, T value)
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
List<T>::List()
{
    head = NULL;
    tail = NULL;
    count = 0;
}

template <typename T>
List<T>::List(List<T> *value)
{
    head = value->first();
    tail = value->last();
    count = value->size();

    value = NULL;
}

template <typename T>
List<T>::~List()
{
    clear();
}

/*
 * CAPACIDAD
*/
template <typename T>
bool List<T>::isEmpty()
{
    return head == NULL;
}

template <typename T>
int List<T>::size()
{
    return count;
}

template <typename T>
void List<T>::clear()
{
    while (!isEmpty())
        pop_front();
}

/*
 * MODIFICADORES
*/
template <typename T>
Node<T> *List<T>::push_front(T value)
{
    Node<T> *node = new Node<T>(value);

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
Node<T> *List<T>::push_back(T value)
{
    Node<T> *node = new Node<T>(value);

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
void List<T>::pop_front()
{
    if (isEmpty())
        return;

    Node<T> *temporal = head;

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
void List<T>::pop_back()
{
    if (isEmpty())
        return;

    Node<T> *temporal = tail;

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
Node<T> *List<T>::insert(T value)
{
    if (isEmpty())
    {
        Node<T> *temporal = new Node<T>(value);
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
void List<T>::erase(T value)
{
    Node<T> *temporal = removeOne(value);
    delete temporal;
    temporal = NULL;
}

template <typename T>
Node<T> *List<T>::removeFirst()
{
    if (isEmpty())
        return NULL;

    Node<T> *temporal = head;
    head = temporal->getNext();
    if (size() > 1)
        head->setPreview(NULL);
    else
        tail = head;
    count--;

    return temporal;
}

template <typename T>
Node<T> *List<T>::removeLast()
{
    if (isEmpty())
        return NULL;

    Node<T> *temporal = tail;
    tail = temporal->getPreview();
    if (size() > 1)
        tail->setNext(NULL);
    else
        head = tail;
    count--;

    return temporal;
}

template <typename T>
Node<T> *List<T>::removeOne(T value)
{
    if (head->getData()->compare(value) == 0)
        return removeFirst();
    if (tail->getData()->compare(value) == 0)
        return removeLast();

    if (head->getData()->compare(value) < 0
            && tail->getData()->compare(value) > 0)
    {
        Node<T> *temporal = get(head->getNext(), value);

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
Node<T> *List<T>::first()
{
    return head;
}

template <typename T>
Node<T> *List<T>::last()
{
    return tail;
}

template <typename T>
T List<T>::front()
{
    return head->getData();
}

template <typename T>
T List<T>::back()
{
    return tail->getData();
}

template <typename T>
Node<T> *List<T>::get(T value)
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
QString List<T>::graph()
{
    QString listGraph;

    Node<T> *temporal = head;
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
void List<T>::sort()
{
    Node<T> *i = NULL;

    for (i = head->getNext(); i != NULL; i = i->getNext())
    {
        Node<T> *j = i->getPreview();
        while (j != NULL && j->getData()->compare(i->getData()) > 0)
        {

        }
    }
}

/* ORDENAMIENTO ? */
template <typename T>
void List<T>::reverse()
{
}

#ifndef LIST_H
#define LIST_H
#include "node.h"

template <typename T>
class List
{
    int count;
    Node<T> *head;
    Node<T> *tail;

    Node<T> *insert(Node<T> *current, T value)
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
    Node<T> *get(Node<T> *current, T value)
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
public:
    List()
    {
        head = NULL;
        tail = NULL;
        count = 0;
    }
    List(List<T> *value)
    {
        head = value->first();
        tail = value->last();
        count = value->size();

        value = NULL;
    }
    ~List()
    {
        clear();
    }

    /* CAPACIDAD */
    bool isEmpty()
    {
        return head == NULL;
    }
    int size()
    {
        return count;
    }
    void clear()
    {
        while (!isEmpty())
            pop_front();
    }

    /* MODIFICADORES */
    Node<T> *push_front(T value)
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
    Node<T> *push_back(T value)
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
    void pop_front()
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
    void pop_back()
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
    Node<T> *insert(T value)
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
    void erase(T value)
    {
        Node<T> *temporal = removeOne(value);
        delete temporal;
        temporal = NULL;
    }
    Node<T> *removeFirst()
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
    Node<T> *removeLast()
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
    Node<T> *removeOne(T value)
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

    /* OPERACIONES */
    Node<T> *first()
    {
        return head;
    }
    Node<T> *last()
    {
        return tail;
    }
    T front()
    {
        return head->getData();
    }
    T back()
    {
        return tail->getData();
    }
    Node<T> *get(T value)
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
    QString graph()
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
    void sort()
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
    void reverse()
    {
    }
};

#endif // LIST_H

#include "nodegenerictree.h"

NodeGenericTree::NodeGenericTree()
{
    data = NULL;
    childs = NULL;
}

NodeGenericTree::~NodeGenericTree()
{
    if (data != NULL)
        delete data;
    delete childs;

    data = NULL;
    childs = NULL;
}

void NodeGenericTree::setData(TADGenericTree *_data)
{
    data = _data;
}

TADGenericTree *NodeGenericTree::getData()
{
    return data;
}

void NodeGenericTree::addChild(NodeGenericTree *child)
{
    childs->insert(child);
}

void NodeGenericTree::setChilds(List<NodeGenericTree *> *value)
{
    childs = value;
}

List<NodeGenericTree *> *NodeGenericTree::getChilds()
{
    return childs;
}

int NodeGenericTree::compare(NodeGenericTree *value)
{
    return data->compare(value->getData());
}

QString NodeGenericTree::toString()
{
    return data->toString();
}

QString NodeGenericTree::getNodeName()
{
    return data->getNodeName();
}

QString NodeGenericTree::createNode()
{
    return data->createNode();
}

QString NodeGenericTree::pointNode(TADGenericTree *next)
{
    return data->pointNode(next);
}

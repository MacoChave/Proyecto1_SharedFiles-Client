#ifndef NODEGENERICTREE_H
#define NODEGENERICTREE_H
#include "List/list.h"
#include "tadgenerictree.h"

class NodeGenericTree
{
    TADGenericTree *data;
    List<NodeGenericTree *> *childs;
public:
    NodeGenericTree();
    NodeGenericTree(TADGenericTree *_data) : data(_data), childs(NULL) {}
    ~NodeGenericTree();

    void setData(TADGenericTree *_data);
    TADGenericTree *getData();
    void addChild(NodeGenericTree *child);
    void setChilds(List<NodeGenericTree *> *value);
    List<NodeGenericTree *> *getChilds();
    int compare(NodeGenericTree *value);
    QString toString();
    QString getNodeName();
    QString createNode();
    QString pointNode(TADGenericTree *next);
};

#endif // NODEGENERICTREE_H

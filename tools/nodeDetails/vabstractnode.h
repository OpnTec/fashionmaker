#ifndef VABSTRACTNODE_H
#define VABSTRACTNODE_H

#include "../vabstracttool.h"

class VAbstractNode : public VAbstractTool
{
    Q_OBJECT
public:
                 VAbstractNode(VDomDocument *doc, VContainer *data, qint64 id, qint64 idNode,
                               Draw::Mode typeobject, QObject *parent = 0 );
    virtual      ~VAbstractNode();
protected:
    qint64       idNode;
    Draw::Mode   typeobject;
    void         AddToModeling(const QDomElement &domElement);
};

#endif // VABSTRACTNODE_H

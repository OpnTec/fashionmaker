#ifndef VMODELINGPOINT_H
#define VMODELINGPOINT_H

#include "vtoolpoint.h"

class VModelingPoint: public VToolPoint
{
    Q_OBJECT
public:
    VModelingPoint(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint, Draw::Mode typeobject,
                   Tool::Enum typeCreation, QGraphicsItem * parent = 0 );
    static VModelingPoint *Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint,
                                  Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void            FullUpdateFromFile();
    virtual void            FullUpdateFromGui(int result);
protected:
    virtual void            AddToFile();
private:
    qint64 idPoint;
    Draw::Mode typeobject;
};

#endif // VMODELINGPOINT_H

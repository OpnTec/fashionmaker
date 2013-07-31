#ifndef VTOOLLINEINTERSECT_H
#define VTOOLLINEINTERSECT_H

#include "vtoolpoint.h"
#include "../dialogs/dialoglineintersect.h"

class VToolLineIntersect:public VToolPoint
{
public:
                                       VToolLineIntersect(VDomDocument *doc, VContainer *data,
                                                          const qint64 &id, const qint64 &p1Line1,
                                                          const qint64 &p2Line1, const qint64 &p1Line2,
                                                          const qint64 &p2Line2, Tool::Enum typeCreation,
                                                          QGraphicsItem * parent = 0);
public slots:
    virtual void                        FullUpdateFromFile();
    virtual void                        FullUpdateFromGui(int result);
protected:
    virtual void                        contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                        AddToFile();
private:
    qint64                              p1Line1;
    qint64                              p2Line1;
    qint64                              p1Line2;
    qint64                              p2Line2;
    QSharedPointer<DialogLineIntersect> dialogLineIntersect;
};

#endif // VTOOLLINEINTERSECT_H

#ifndef VTOOLSIMPLEPOINT_H
#define VTOOLSIMPLEPOINT_H

#include "../container/vcontainer.h"
#include "../xml/vdomdocument.h"
#include "vtoolpoint.h"

class VToolSimplePoint : public VToolPoint
{
    Q_OBJECT
public:
                            VToolSimplePoint (VDomDocument *doc, VContainer *data, qint64 id,
                                              Tool::Enum typeCreation, QGraphicsItem * parent = 0 );
public slots:
    virtual void            ChangedActivDraw(const QString newName);
    virtual void            FullUpdateFromFile();
signals:
    void                    FullUpdateTree();
protected:
    virtual void            contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void            AddToFile();
private:
    bool                    ignoreContextMenuEvent;

    void                    FullUpdateFromGui(const QString& name, qreal x, qreal y);
};

#endif // VTOOLSIMPLEPOINT_H

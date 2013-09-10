#ifndef VMODELINGSINGLEPOINT_H
#define VMODELINGSINGLEPOINT_H

#include "container/vcontainer.h"
#include "xml/vdomdocument.h"
#include "dialogs/dialogsinglepoint.h"
#include "vmodelingpoint.h"

class VModelingSinglePoint : public VModelingPoint
{
    Q_OBJECT
public:
                                      VModelingSinglePoint (VDomDocument *doc, VContainer *data, qint64 id,
                                                        Tool::Enum typeCreation, QGraphicsItem * parent = 0 );
    virtual void                      setDialog();
public slots:
    virtual void                      FullUpdateFromFile();
    virtual void                      FullUpdateFromGui(int result);
signals:
    void                              FullUpdateTree();
protected:
    virtual void                      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                      AddToFile();
    QVariant                          itemChange ( GraphicsItemChange change, const QVariant &value );
private:
    QSharedPointer<DialogSinglePoint> dialogSinglePoint;
};

#endif // VMODELINGSINGLEPOINT_H

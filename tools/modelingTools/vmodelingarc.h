#ifndef VMODELINGARC_H
#define VMODELINGARC_H

#include "vmodelingtool.h"
#include "xml/vdomdocument.h"
#include <QGraphicsPathItem>
#include "dialogs/dialogarc.h"
#include "widgets/vcontrolpointspline.h"
#include "container/vcontainer.h"

class VModelingArc :public VModelingTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VModelingArc(VDomDocument *doc, VContainer *data, qint64 id, Tool::Enum typeCreation,
             QGraphicsItem * parent = 0);
    virtual void setDialog();
    static VModelingArc* Create(QSharedPointer<DialogArc> &dialog, VDomDocument *doc, VContainer *data);
    static VModelingArc* Create(const qint64 _id, const qint64 &center, const QString &radius,
                       const QString &f1, const QString &f2, VDomDocument *doc, VContainer *data,
                                Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void              FullUpdateFromFile();
    virtual void              FullUpdateFromGui(int result);
protected:
    virtual void              contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void              AddToFile();
    virtual void              mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void              hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void              hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    QSharedPointer<DialogArc> dialogArc;
    void                      RefreshGeometry();
};

#endif // VMODELINGARC_H

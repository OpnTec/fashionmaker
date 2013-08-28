#ifndef VTOOLARC_H
#define VTOOLARC_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "vabstracttool.h"
#include "../xml/vdomdocument.h"
#include <QGraphicsPathItem>
#include "../dialogs/dialogarc.h"
#include "../widgets/vcontrolpointspline.h"
#pragma GCC diagnostic pop
#include "../container/vcontainer.h"

class VToolArc :public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VToolArc(VDomDocument *doc, VContainer *data, qint64 id, Draw::Mode mode, Tool::Enum typeCreation,
             QGraphicsItem * parent = 0);
    virtual void setDialog();
    static void Create(QSharedPointer<DialogArc> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                       VContainer *data, Draw::Mode mode);
    static void Create(const qint64 _id, const qint64 &center, const QString &radius,
                       const QString &f1, const QString &f2, VMainGraphicsScene  *scene, VDomDocument *doc,
                       VContainer *data, Document::Enum parse, Tool::Enum typeCreation, Draw::Mode mode);
public slots:
    virtual void                 FullUpdateFromFile();
    virtual void                 FullUpdateFromGui(int result);
    virtual void                 ChangedActivDraw(const QString newName);
    virtual void                 ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
protected:
    virtual void                 contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                 AddToFile();
    virtual void                 mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void                 hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void                 hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    QSharedPointer<DialogArc> dialogArc;
    void                         RefreshGeometry();
};

#endif // VTOOLARC_H

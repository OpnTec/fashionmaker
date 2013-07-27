#ifndef VTOOLLINE_H
#define VTOOLLINE_H

#include "vabstracttool.h"
#include "QGraphicsLineItem"
#include "../dialogs/dialogline.h"

class VToolLine: public VAbstractTool, public QGraphicsLineItem
{
    Q_OBJECT
public:
    VToolLine(VDomDocument *doc, VContainer *data, qint64 id, qint64 firstPoint, qint64 secondPoint,
              Tool::Enum typeCreation, QGraphicsItem * parent = 0);
public slots:
    virtual void      FullUpdateFromFile();
    virtual void      ChangedActivDraw(const QString newName);
    virtual void      FullUpdateFromGui(int result);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
    virtual void      hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void      hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    qint64            firstPoint;
    qint64            secondPoint;
    QSharedPointer<DialogLine> dialogLine;
};

#endif // VTOOLLINE_H

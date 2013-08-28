#ifndef VTOOLLINE_H
#define VTOOLLINE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "vabstracttool.h"
#include "QGraphicsLineItem"
#include "../dialogs/dialogline.h"
#pragma GCC diagnostic pop

class VToolLine: public VAbstractTool, public QGraphicsLineItem
{
    Q_OBJECT
public:
    VToolLine(VDomDocument *doc, VContainer *data, qint64 id, Draw::Mode mode, qint64 firstPoint,
              qint64 secondPoint, Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    virtual void setDialog();
    static void Create(QSharedPointer<DialogLine> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                       VContainer *data, Draw::Mode mode);
    static void Create(const qint64 &id, const qint64 &firstPoint, const qint64 &secondPoint,
                       VMainGraphicsScene  *scene, VDomDocument *doc, VContainer *data, Document::Enum parse,
                       Tool::Enum typeCreation, Draw::Mode mode);
public slots:
    virtual void      FullUpdateFromFile();
    virtual void      ChangedActivDraw(const QString newName);
    virtual void      FullUpdateFromGui(int result);
    virtual void      ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
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

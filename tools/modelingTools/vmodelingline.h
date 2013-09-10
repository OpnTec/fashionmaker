#ifndef VMODELINGLINE_H
#define VMODELINGLINE_H

#include "vmodelingtool.h"
#include "QGraphicsLineItem"
#include "dialogs/dialogline.h"

class VModelingLine: public VModelingTool, public QGraphicsLineItem
{
    Q_OBJECT
public:
    VModelingLine(VDomDocument *doc, VContainer *data, qint64 id, qint64 firstPoint,
              qint64 secondPoint, Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    virtual void setDialog();
    static VModelingLine* Create(QSharedPointer<DialogLine> &dialog, VDomDocument *doc, VContainer *data);
    static VModelingLine* Create(const qint64 &id, const qint64 &firstPoint, const qint64 &secondPoint,
                                 VDomDocument *doc, VContainer *data, Document::Enum parse,
                                 Tool::Enum typeCreation);
public slots:
    virtual void      FullUpdateFromFile();
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

#endif // VMODELINGLINE_H

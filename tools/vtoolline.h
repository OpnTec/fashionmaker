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
              Tool::Enum typeCreation);
public slots:
    virtual void      FullUpdateFromFile();
    void              ClosedDialogLine(int result);
    virtual void      ChangedActivDraw(const QString newName);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
private:
    qint64            firstPoint;
    qint64            secondPoint;
    QSharedPointer<DialogLine> dialogLine;
    void              FullUpdateFromGui(qint64 firstPoint, qint64 secondPoint);
};

#endif // VTOOLLINE_H

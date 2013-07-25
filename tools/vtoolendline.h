#ifndef VTOOLENDLINE_H
#define VTOOLENDLINE_H

#include <QSharedPointer>

#include "vtoolpoint.h"
#include "../dialogs/dialogendline.h"

class VToolEndLine : public VToolPoint
{
    Q_OBJECT
public:
                      VToolEndLine(VDomDocument *doc, VContainer *data, const qint64 &id,
                                   const QString &typeLine, const QString &formula, const qint32 &angle,
                                   const qint64 &basePointId, Tool::Enum typeCreation,
                                   QGraphicsItem * parent = 0);
public slots:
    virtual void      FullUpdateFromFile();
    void              ClosedDialogEndLine(int result);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
private:
    QString           typeLine;
    QString           formula;
    qint32            angle;
    qint64            basePointId;
    QGraphicsLineItem *mainLine;
    QSharedPointer<DialogEndLine> dialogEndLine;
    void              FullUpdateFromGui(const QString &name, const QString &typeLine, const QString &formula,
                                        const qreal &angle, const qint64 &basePointId);
};

#endif // VTOOLENDLINE_H

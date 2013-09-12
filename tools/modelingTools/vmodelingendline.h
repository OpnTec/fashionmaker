#ifndef VMODELINGENDLINE_H
#define VMODELINGENDLINE_H

#include <QSharedPointer>
#include "vmodelinglinepoint.h"
#include "dialogs/dialogendline.h"

class VModelingEndLine : public VModelingLinePoint
{
    Q_OBJECT
public:
                      VModelingEndLine(VDomDocument *doc, VContainer *data, const qint64 &id,
                                   const QString &typeLine, const QString &formula, const qreal &angle,
                                   const qint64 &basePointId, Tool::Enum typeCreation,
                                   QGraphicsItem * parent = 0);
    virtual void setDialog();
    static VModelingEndLine* Create(QSharedPointer<DialogEndLine> &dialog, VDomDocument *doc,
                                    VContainer *data);
    static VModelingEndLine* Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                                    const QString &formula, const qreal &angle, const qint64 &basePointId,
                                    const qreal &mx, const qreal &my, VDomDocument *doc, VContainer *data,
                                    Document::Enum parse, Tool::Enum typeCreation);
public slots:
    virtual void      FullUpdateFromFile();
    virtual void      FullUpdateFromGui(int result);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
private:
    QSharedPointer<DialogEndLine> dialogEndLine;
};

#endif // VMODELINGENDLINE_H

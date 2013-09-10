#ifndef VMODELINGALONGLINE_H
#define VMODELINGALONGLINE_H

#include "vmodelinglinepoint.h"
#include "dialogs/dialogalongline.h"

class VModelingAlongLine : public VModelingLinePoint
{
    Q_OBJECT
public:
    VModelingAlongLine(VDomDocument *doc, VContainer *data, qint64 id, const QString &formula,
                   const qint64 &firstPointId, const qint64 &secondPointId, const QString &typeLine,
                   Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    virtual void      setDialog();
    static VModelingAlongLine* Create(QSharedPointer<DialogAlongLine> &dialog, VDomDocument *doc,
                                      VContainer *data);
    static VModelingAlongLine* Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                                      const QString &formula, const qint64 &firstPointId,
                                      const qint64 &secondPointId, const qreal &mx, const qreal &my,
                                      VDomDocument *doc, VContainer *data, Document::Enum parse,
                                      Tool::Enum typeCreation);
public slots:
    virtual void      FullUpdateFromFile();
    virtual void      FullUpdateFromGui(int result);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();

private:
    qint64            secondPointId;
    QSharedPointer<DialogAlongLine> dialogAlongLine;
};

#endif // VMODELINGALONGLINE_H

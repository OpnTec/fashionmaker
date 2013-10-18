#ifndef VMODELINGHEIGHT_H
#define VMODELINGHEIGHT_H

#include "vmodelinglinepoint.h"
#include "dialogs/dialogheight.h"

class VModelingHeight : public VModelingLinePoint{
    Q_OBJECT
public:
                            VModelingHeight(VDomDocument *doc, VContainer *data, const qint64 &id,
                                            const QString &typeLine, const qint64 &basePointId,
                                            const qint64 &p1LineId, const qint64 &p2LineId,
                                            Tool::Sources typeCreation, QGraphicsItem * parent = 0);
    virtual void            setDialog();
    static VModelingHeight* Create(QSharedPointer<DialogHeight> &dialog, VDomDocument *doc,
                                   VContainer *data);
    static VModelingHeight* Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                                   const qint64 &basePointId, const qint64 &p1LineId, const qint64 &p2LineId,
                                   const qreal &mx, const qreal &my, VDomDocument *doc, VContainer *data,
                                   const Document::Documents &parse, Tool::Sources typeCreation);
public slots:
    virtual void            FullUpdateFromFile();
    virtual void            FullUpdateFromGui(int result);
protected:
    virtual void            contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void            AddToFile();
private:
    QSharedPointer<DialogHeight> dialogHeight;
    qint64                  p1LineId;
    qint64                  p2LineId;
};

#endif // VMODELINGHEIGHT_H

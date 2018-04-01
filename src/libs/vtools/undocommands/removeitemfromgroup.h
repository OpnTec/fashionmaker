#ifndef REMOVEITEMFROMGROUP_H
#define REMOVEITEMFROMGROUP_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "vundocommand.h"

class RemoveItemFromGroup : public VUndoCommand
{
    Q_OBJECT
public:
    RemoveItemFromGroup(const QDomElement &xml, VAbstractPattern *doc, quint32 groupId, QUndoCommand *parent = nullptr);
    virtual ~RemoveItemFromGroup();
    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
signals:
    void UpdateGroups();
private:
    Q_DISABLE_COPY(RemoveItemFromGroup)
    const QString nameActivDraw;
};

#endif // REMOVEITEMFROMGROUP_H

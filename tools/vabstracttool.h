#ifndef VABSTRACTTOOL_H
#define VABSTRACTTOOL_H

#include <QObject>

#include "../container/vcontainer.h"
#include "../xml/vdomdocument.h"

namespace Tool{
    enum Enum
    {
        FromGui,
        FromFile
    };
}

class VAbstractTool:public QObject
{
    Q_OBJECT
public:
                 VAbstractTool(VDomDocument *doc, VContainer *data, qint64 id);
    virtual      ~VAbstractTool();
public slots:
    virtual void FullUpdateFromFile()=0;
    void         ChangedNameDraw(const QString oldName, const QString newName);
    virtual void ChangedActivDraw(const QString newName);
signals:
    void         haveLiteChange();
    void         ChoosedPoint(qint64 id, Scene::Type type);
    void         FullUpdateTree();
protected:
    VDomDocument *doc;
    VContainer   *data;
    qint64       id;
    bool         ignoreContextMenuEvent;
    QString      nameActivDraw;
    virtual void AddToFile()=0;
    void         AddAttribute(QDomElement &domElement, const QString &name, const qint64 &value);
    void         AddAttribute(QDomElement &domElement, const QString &name, const qint32 &value);
    void         AddAttribute(QDomElement &domElement, const QString &name, const qreal &value);
    void         AddAttribute(QDomElement &domElement, const QString &name, const QString &value);
    QString      GetNameLine(qint64 firstPoint, qint64 secondPoint) const;
};
#endif // VABSTRACTTOOL_H

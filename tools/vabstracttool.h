#ifndef VABSTRACTTOOL_H
#define VABSTRACTTOOL_H

#include "../xml/vdomdocument.h"
#include "vdatatool.h"
#include "../container/vcontainer.h"

namespace Tool{
    enum Enum
    {
        FromGui,
        FromFile
    };
}

class VAbstractTool:public VDataTool
{
    Q_OBJECT
public:
                          VAbstractTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent = 0);
    virtual               ~VAbstractTool();
    static QPointF        LineIntersectRect(QRectF rec, QLineF line);
    static qint32         LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
                                              QPointF &p2);
    static QPointF        ClosestPoint(QLineF line, QPointF p);
    static QPointF        addVector (QPointF p, QPointF p1, QPointF p2, qreal k);
    qint64 getId() const;

public slots:
    virtual void          FullUpdateFromFile()=0;
signals:
    void                  toolhaveChange();
    void                  ChoosedTool(qint64 id, Scene::Type type);
    void                  FullUpdateTree();
protected:
    VDomDocument          *doc;
    qint64                id;
    const Qt::GlobalColor baseColor;
    Qt::GlobalColor       currentColor;
    virtual void          AddToFile()=0;
    void                  AddAttribute(QDomElement &domElement, const QString &name, const qint64 &value);
    void                  AddAttribute(QDomElement &domElement, const QString &name, const qint32 &value);
    void                  AddAttribute(QDomElement &domElement, const QString &name, const qreal &value);
    void                  AddAttribute(QDomElement &domElement, const QString &name, const QString &value);
    const VContainer      *getData() const;
private:
                          VAbstractTool(const VAbstractTool &tool);
    const VAbstractTool   &operator=(const VAbstractTool &tool);
};
#endif // VABSTRACTTOOL_H

#ifndef VDETAIL_H
#define VDETAIL_H

#include "vnodedetail.h"
#include <QVector>
#include <QString>
#include <QPainterPath>

namespace Detail{
    enum Contour
    {
        OpenContour,
        CloseContour
    };

    enum Equidistant
    {
        OpenEquidistant,
        CloseEquidistant
    };
}

class VDetail
{
public:
    VDetail();
    VDetail(const QString &name, const QVector<VNodeDetail> &nodes);
    void append(const VNodeDetail &node);
    void Clear();
    qint32 CountNode() const;
    bool Containes(const qint64 &id)const;
    VNodeDetail & operator[](int indx);
    QString getName() const;
    void setName(const QString &value);

    qreal getMx() const;
    void setMx(const qreal &value);

    qreal getMy() const;
    void setMy(const qreal &value);

private:
    QVector<VNodeDetail> nodes;
    QString name;
    qreal mx;
    qreal my;
};

#endif // VDETAIL_H

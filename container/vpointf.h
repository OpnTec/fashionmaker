#ifndef VPOINTF_H
#define VPOINTF_H

#include <QPointF>
#include <QString>
#include "options.h"

class VPointF
{
public:
    VPointF();
    VPointF (const VPointF &point );
    VPointF ( qreal x, qreal y, QString name, qreal mx, qreal my, Draw::Mode mode = Draw::Calculation,
              qint64 idObject = 0);
    ~VPointF();
    QString name() const;
    qreal mx() const;
    qreal my() const;
    void setName(const QString &name);
    void setMx(qreal mx);
    void setMy(qreal my);
    QPointF toQPointF()const;
    qreal x() const;
    void setX(const qreal &value);
    qreal y() const;
    void setY(const qreal &value);
    qint32 referens() const;
    void incrementReferens();
    void decrementReferens();
    Draw::Mode getMode() const;
    void setMode(const Draw::Mode &value);

    qint64 getIdObject() const;
    void setIdObject(const qint64 &value);

private:
    QString _name;
    qreal _mx;
    qreal _my;
    qreal _x;
    qreal _y;
    qint32 _referens;
    Draw::Mode mode;
    qint64 idObject;
};

#endif // VPOINTF_H

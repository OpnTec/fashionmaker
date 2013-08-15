#ifndef VPOINTF_H
#define VPOINTF_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <QPointF>
#include <QString>
#pragma GCC diagnostic pop

class VPointF
{
public:
    VPointF();
    VPointF (const VPointF &point );
    VPointF ( qreal x, qreal y, QString name, qreal mx, qreal my );
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

private:
    QString _name;
    qreal _mx;
    qreal _my;
    qreal _x;
    qreal _y;
};

#endif // VPOINTF_H

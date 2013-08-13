#ifndef VSPLINEPATH_H
#define VSPLINEPATH_H

#include "vsplinepoint.h"
#pragma GCC diagnostic ignored "-Weffc++"
#include <QVector>
#pragma GCC diagnostic warning "-Weffc++"
#include "vspline.h"

namespace SplinePoint{
    enum Position
    {
        FirstPoint,
        LastPoint
    };
}

/**
 * @brief The VSplinePath клас, що розраховує шлях сплайнів.
 */
class VSplinePath{
public:
                          /**
                           * @brief VSplinePath конструктор по замовчуванню.
                           */
                          VSplinePath();
                          /**
                           * @brief VSplinePath конструктор по замовчуванню.
                           */
                          VSplinePath(const QMap<qint64, VPointF> *points, qreal kCurve = 1);
    /**
     * @brief append додає точку сплайну до шляху.
     * @param point точка.
     */
    void                  append(VSplinePoint point);
    qint32                Count() const;
    qint32                CountPoint() const;
    VSpline               GetSpline(qint32 index) const;
    QPainterPath          GetPath() const;
    QVector<VSplinePoint> GetSplinePath() const;
    qreal                 GetLength() const;
    const QMap<qint64, VPointF> *GetDataPoints() const;
    void                  UpdatePoint(qint32 indexSpline, SplinePoint::Position pos, VSplinePoint point);
    VSplinePoint          GetSplinePoint(qint32 indexSpline, SplinePoint::Position pos);
    /**
     * @brief Clear очищає шлях сплайнів.
     */
    void                  Clear();
    qreal getKCurve() const;
    void setKCurve(const qreal &value);
    const QVector<VSplinePoint> *GetPoint() const;
    VSplinePath& operator=(const VSplinePath &path);
    VSplinePoint & operator[](int indx);
protected:
    /**
     * @brief path вектор з точок сплайна.
     */
    QVector<VSplinePoint> path;
    qreal kCurve;
    const QMap<qint64, VPointF> *points;
};

#endif // VSPLINEPATH_H

#ifndef VSPLINEPATH_H
#define VSPLINEPATH_H

#include "vsplinepoint.h"
#include <QVector>
#include "vspline.h"
#include "options.h"

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
                          VSplinePath(const QMap<qint64, VPointF> *points, qreal kCurve = 1,
                                      Draw::Mode mode = Draw::Calculation, qint64 idObject = 0);
                          VSplinePath(const VSplinePath& splPath);
    /**
     * @brief append додає точку сплайну до шляху.
     * @param point точка.
     */
    void                  append(VSplinePoint point);
    qint32                Count() const;
    qint32                CountPoint() const;
    VSpline               GetSpline(qint32 index) const;
    QPainterPath          GetPath() const;
    QVector<QPointF>      GetPathPoints() const;
    QVector<VSplinePoint> GetSplinePath() const;
    qreal                 GetLength() const;
    const QMap<qint64, VPointF> *GetDataPoints() const;
    void                  UpdatePoint(qint32 indexSpline, SplinePoint::Position pos, VSplinePoint point);
    VSplinePoint          GetSplinePoint(qint32 indexSpline, SplinePoint::Position pos) const;
    /**
     * @brief Clear очищає шлях сплайнів.
     */
    void                  Clear();
    qreal getKCurve() const;
    void setKCurve(const qreal &value);
    const QVector<VSplinePoint> *GetPoint() const;
    VSplinePath& operator=(const VSplinePath &path);
    VSplinePoint & operator[](int indx);
    qint32 referens() const;
    void incrementReferens();
    void decrementReferens();
    Draw::Mode getMode() const;
    void setMode(const Draw::Mode &value);

    qint64 getIdObject() const;
    void setIdObject(const qint64 &value);

protected:
    /**
     * @brief path вектор з точок сплайна.
     */
    QVector<VSplinePoint> path;
    qreal kCurve;
    Draw::Mode mode;
    const QMap<qint64, VPointF> *points;
    qint32 _referens;
    qint64 idObject;
};

#endif // VSPLINEPATH_H

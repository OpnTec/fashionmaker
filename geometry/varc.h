#ifndef VARC_H
#define VARC_H

#include <QPainterPath>
#include <QVector>
#include <QMap>
#include "container/vpointf.h"
#include "vspline.h"

/**
 * @brief VArc клас, що реалізує дугу. Дуга розраховується за годиниковою стрілкою.
 */
class VArc{
public:
                 /**
                  * @brief VArc конструктор по замовчуванню.
                  */
                 VArc ();
                 /**
                  * @brief VArc конструктор.
                  * @param center точка центру.
                  * @param radius радіус.
                  * @param f1 початковий кут в градусах.
                  * @param f2 кінцевий кут в градусах.
                  */
                 VArc (const QMap<qint64, VPointF> *points, qint64 center, qreal radius, QString formulaRadius,
                       qreal f1, QString formulaF1, qreal f2 , QString formulaF2,
                       Draw::Mode mode = Draw::Calculation, qint64 idObject = 0);
                 VArc(const VArc &arc);
                 VArc& operator= (const VArc &arc);
    /**
     * @brief GetF1 повертає початковий кут дуги.
     * @return повертає кут в градусах.
     */
    QString      GetFormulaF1 () const;
    qreal        GetF1 () const;
    /**
     * @brief GetF2 повертає кінцевий кут дуги.
     * @return повертає кут в градусах.
     */
    QString      GetFormulaF2 () const;\
    qreal        GetF2 () const;
    /**
     * @brief GetLength повертає довжину дуги.
     * @return повертає довжину дуги.
     */
    qreal        GetLength () const;
    /**
     * @brief GetRadius повертає радіус дуги.
     * @return повертає радіус дуги.
     */
    QString      GetFormulaRadius () const;
    qreal        GetRadius () const;
    /**
     * @brief GetCenter повертає точку центра дуги.
     * @return повертає точку центра дуги.
     */
    qint64      GetCenter () const;
    QPointF GetCenterPoint() const;
    /**
     * @brief GetP1 повертає першу точку з якої починається дуга.
     * @return точку початку дуги.
     */
    QPointF      GetP1 () const;
    /**
     * @brief GetP2 повертає другу точку в якій закінчується дуга.
     * @return точку кінця дуги.
     */
    QPointF      GetP2 () const;
    const QMap<qint64, VPointF> *GetDataPoints() const;
    /**
     * @brief GetPath будує шлях по даній дузі.
     * @return повертає шлях.
     */
    QPainterPath GetPath() const;
    qreal AngleArc()const;
    qint32 NumberSplOfArc () const;
    QVector<QPointF> GetPoints () const;
    QVector<QPointF> SplOfArc( qint32 number ) const;
    qint32 referens() const;
    void incrementReferens();
    void decrementReferens();
    Draw::Mode getMode() const;
    void setMode(const Draw::Mode &value);

    qint64 getIdObject() const;
    void setIdObject(const qint64 &value);

private:
    /**
     * @brief f1 початковий кут в градусах
     */
    qreal   f1;			// початковий кут нахилу дуги (градуси)
    QString formulaF1;
    /**
     * @brief f2 кінцевий кут в градусах
     */
    qreal   f2;			// кінцевий кут нахилу дуги (градуси)
    QString formulaF2;
    /**
     * @brief radius радіус дуги.
     */
    qreal   radius;
    QString formulaRadius;
    /**
     * @brief center центральна точка дуги.
     */
    qint64 center;
    const QMap<qint64, VPointF> *points;
    qint32 _referens;
    Draw::Mode mode;
    qint64 idObject;
};

#endif // VARC_H

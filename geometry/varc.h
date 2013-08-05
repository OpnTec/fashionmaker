#ifndef VARC_H
#define VARC_H

#include "vspline.h"
#include <QPainterPath>
#include <QVector>
#include <QMap>
#include "container/vpointf.h"

/**
 * @brief VArc клас, що реалізує дугу. Дуга розраховується за годиниковою стрілкою.
 */
class VArc{
public:
    enum Tangent_e { FirstTangent, SecondTangent };
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
                 VArc ( const QMap<qint64, VPointF> *points, qint64 center, qreal radius, qreal f1,
                        qreal f2 );
    /**
     * @brief GetF1 повертає початковий кут дуги.
     * @return повертає кут в градусах.
     */
    qreal        GetF1 () const;
    /**
     * @brief GetF2 повертає кінцевий кут дуги.
     * @return повертає кут в градусах.
     */
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
     * @brief ModifiArc змінює параметри дуги.
     * @param center новий центр дуги.
     * @param radius новий радіус дуги.
     * @param f1 новий початковий кут в градусах.
     * @param f2 новий кінцевий кут в градусах.
     */
    void         ModifiArc (qint64 center, qreal radius, qreal f1, qreal f2 );
    /**
     * @brief BiasArc зміщує дугу.
     * @param mx зміщення по х координаті.
     * @param my зміщення по у координаті.
     */
//    void         BiasArc ( qreal mx, qreal my );
    /**
     * @brief RotationArc повертає дугу на кут проти годиникової стрілки відносно точки.
     * @param pRotate точка відносно якої повертається.
     * @param angle кут в градусах на який повертається в градусах.
     */
//    void         RotationArc ( QPointF pRotate, qreal angle );
    /**
     * @brief SplOfArc повертає сплайн дуги за номером.
     * @param number номер сплайна в дузі.
     * @return сплайн.
     */
    VSpline      SplOfArc ( qint32 number ) const;
    /**
     * @brief CutArc розрізає дугу на дві дуги.
     * @param length довжина першої дуги.
     * @param arcFir перша дуга.
     * @param arcSec друга дуга.
     */
//    void         CutArc ( qreal length, VArc &arcFir, VArc &arcSec ) const;
    /**
     * @brief CutArc розрізає дугу на дві дуги.
     * @param point точка яка ділить дугу.
     * @param arcFir перша дуга.
     * @param arcSec друга дуга.
     */
//    void         CutArc ( QPointF point, VArc &arcFir, VArc &arcSec ) const;
    /**
     * @brief CrossingArcLine перевіряє перетин дуги і лінії.
     * @param line лінія з якою шукаємо перетин.
     * @param intersectionPoint точка перетину.
     * @return результат перевірки на перетин.
     */
    QLineF::IntersectType CrossingArcLine(const QLineF &line, QPointF *intersectionPoint) const;
    /**
     * @brief NumberSplOfArc метод шукає кілкість сплайнів які необхідні для відтворення такої дуги.
     * @return кількість сплайнів.
     */
    qint32       NumberSplOfArc () const;
    /**
     * @brief GetPath будує шлях по даній дузі.
     * @return повертає шлях.
     */
    QPainterPath GetPath() const;
    /**
     * @brief GetPoints повертає набір точок по яких можна побудувати дугу.
     * @return набір точок дуги.
     */
    QVector<QPointF>
                 GetPoints () const;
    /**
     * @brief Tangent розраховує дотичну до дуги.
     * @param P точка через, що не належить дузі, через яку проходить дотична.
     * @param tan перша чи друга дотична.
     * @return точку дотичної на дузі.
     */
    QPointF      Tangent(const QPointF P,VArc::Tangent_e tan = VArc::FirstTangent)const;
    /**
     * @brief Contains перевіряє чи точка належить дузі.
     * @param P точка що перевіряється.
     * @return true - належить дузі, false - не належить дузі.
     */
    bool         Contains(const QPointF P)const;
    /**
     * @brief PutAlongArc розміщує точку на дузі.
     * @param length довжина від початку дуги.
     * @return точка що лежить на дузі.
     */
    QPointF      PutAlongArc(qreal length) const;
    /**
     * @brief Mirror дзеркальне відображення дуги.
     * @param Pmirror точка відносно якої відбувається дзеркалення.
     */
//    void         Mirror(const QPointF Pmirror);
private:
    /**
     * @brief f1 початковий кут в градусах
     */
    qreal   f1;			// початковий кут нахилу дуги (градуси)
    /**
     * @brief f2 кінцевий кут в градусах
     */
    qreal   f2;			// кінцевий кут нахилу дуги (градуси)
    /**
     * @brief length довжина дуги.
     */
    qreal   length;		// довжина дуги
    /**
     * @brief radius радіус дуги.
     */
    qreal   radius;
    /**
     * @brief center центральна точка дуги.
     */
    qint64 center;
    /**
     * @brief p1 точка початку побудови дуги.
     */
    QPointF p1;
    /**
     * @brief p2 кінцева точка побудови дуги.
     */
    QPointF p2;
    const QMap<qint64, VPointF> *points;
    /**
     * @brief AngleArc визначає кут дуги.
     * @param f1 початковий кут дуги в градусах.
     * @param f2 кінцевий кут в градусах.
     * @return повертає кут в градусах.
     */
    qreal   AngleArc(qreal f1, qreal f2 ) const;
};

#endif // VARC_H

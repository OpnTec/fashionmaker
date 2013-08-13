#ifndef VSPLINEPOINT_H
#define VSPLINEPOINT_H

#pragma GCC diagnostic ignored "-Weffc++"
#include "QtGlobal"
#include <QMetaType>
#pragma GCC diagnostic pop

/**
 * @brief The VSplinePoint клас, що містить у собі інформацію про точки сплайну.
 */
class VSplinePoint{
public:
            /**
             * @brief VSplinePoint конструктор по замповчуванню.
             */
            VSplinePoint();
            /**
             * @brief VSplinePoint конструктор.
             * @param pSpline точка сплайну.
             * @param angle кут дотичної сплайна.
             * @param factor коефіцієнт довжини дотичної.
             */
            VSplinePoint(qint64 pSpline, qreal kAsm1, qreal angle, qreal kAsm2);
            VSplinePoint(const VSplinePoint &point);
            ~VSplinePoint();
    /**
     * @brief P повертає точку.
     * @return точка.
     */
    qint64 P() const;
    void SetP(const qint64 &value);
    /**
     * @brief Angle1 повертає кут дотичної сплайна.
     * @return кут в градусах.
     */
    qreal   Angle1() const;
    void    SetAngle(const qreal &value);
    /**
     * @brief Angle2 повертає кут дотичної сплайна.
     * @return кут в градусах.
     */
    qreal   Angle2() const;
    /**
     * @brief KAsm1 повертає коефіцієнт довжини дотичної.
     * @return коефіцієнт.
     */
    qreal   KAsm1() const;
    void    SetKAsm1(const qreal &value);
    /**
     * @brief KAsm2 повертає коефіцієнт довжини дотичної.
     * @return коефіцієнт.
     */
    qreal   KAsm2() const;
    void    SetKAsm2(const qreal &value);
protected:
    /**
     * @brief pSpline точка сплайну.
     */
    qint64 pSpline;
    /**
     * @brief angle кут дотичної сплайну.
     */
    qreal   angle;
    /**
     * @brief kAsm1 коефіцієнт довжини дотичної сплайну.
     */
    qreal   kAsm1;
    /**
     * @brief kAsm2 коефіцієнт довжини дотичної сплайну.
     */
    qreal   kAsm2;
};

Q_DECLARE_METATYPE(VSplinePoint)

#endif // VSPLINEPOINT_H

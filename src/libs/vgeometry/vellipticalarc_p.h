#ifndef VELLIPTICALARC_P
#define VELLIPTICALARC_P

#include <QSharedData>
#include "../vmisc/vabstractapplication.h"
#include "vpointf.h"

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VEllipticalArcData : public QSharedData
{
public:
    VEllipticalArcData ();
    VEllipticalArcData (const VPointF &center, qreal radius1, qreal radius2, const QString &formulaRadius1,
                        const QString &formulaRadius2, qreal f1, const QString &formulaF1, qreal f2,
                        const QString &formulaF2, qreal rotationAngle);
    VEllipticalArcData(const VPointF &center, qreal radius1, qreal radius2, qreal f1, qreal f2, qreal rotationAngle);
    VEllipticalArcData (const QString &formulaLength, const VPointF &center, qreal radius1, qreal radius2,
                        const QString &formulaRadius1, const QString &formulaRadius2, qreal f1,
                        const QString &formulaF1, qreal rotationAngle);
    VEllipticalArcData(const VPointF &center, qreal radius1, qreal radius2, qreal f1, qreal rotationAngle);
    VEllipticalArcData(const VEllipticalArcData &arc);

    virtual ~VEllipticalArcData();

    /** @brief f1 start angle in degree. */
    qreal              f1;
    /** @brief f2 end angle in degree. */
    qreal              f2;
    /** @brief formulaF1 formula for start angle. */
    QString            formulaF1;
    /** @brief formulaF2 formula for end angle. */
    QString            formulaF2;
    /** @brief radius1 elliptical arc major radius. */
    qreal              radius1;
    /** @brief radius2 elliptical arc minor radius. */
    qreal              radius2;
    /** @brief formulaRadius1 formula for elliptical arc major radius. */
    QString            formulaRadius1;
    /** @brief formulaRadius2 formula for elliptical arc minor radius. */
    QString            formulaRadius2;
    /** @brief center center point of arc. */
    VPointF            center;
    bool               isFlipped;
    QString            formulaLength;
    /** @brief rotationAngle in degree. */
    qreal              rotationAngle;

private:
    VEllipticalArcData &operator=(const VEllipticalArcData &) Q_DECL_EQ_DELETE;
};

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::VEllipticalArcData()
    : f1(0),
      f2(0),
      formulaF1(),
      formulaF2(),
      radius1(0),
      radius2(0),
      formulaRadius1(),
      formulaRadius2(),
      center(),
      isFlipped(false),
      formulaLength(),
      rotationAngle(0)
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::VEllipticalArcData(const VPointF &center, qreal radius1, qreal radius2,
                                       const QString &formulaRadius1, const QString &formulaRadius2, qreal f1,
                                       const QString &formulaF1, qreal f2, const QString &formulaF2,
                                       qreal rotationAngle)
    : f1(f1),
      f2(f2),
      formulaF1(formulaF1),
      formulaF2(formulaF2),
      radius1(radius1),
      radius2(radius2),
      formulaRadius1(formulaRadius1),
      formulaRadius2(formulaRadius2),
      center(center),
      isFlipped(false),
      formulaLength(),
      rotationAngle(rotationAngle)
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::VEllipticalArcData(const VPointF &center, qreal radius1, qreal radius2, qreal f1, qreal f2,
                                       qreal rotationAngle)
    : f1(f1),
      f2(f2),
      formulaF1(QString().number(f1)),
      formulaF2(QString().number(f2)),
      radius1(radius1),
      radius2(radius2),
      formulaRadius1(QString().number(qApp->fromPixel(radius1))),
      formulaRadius2(QString().number(qApp->fromPixel(radius2))),
      center(center),
      isFlipped(false),
      formulaLength(),
      rotationAngle(rotationAngle)
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::VEllipticalArcData(const QString &formulaLength, const VPointF &center, qreal radius1,
                                       qreal radius2, const QString &formulaRadius1, const QString &formulaRadius2,
                                       qreal f1, const QString &formulaF1, qreal rotationAngle)
    : f1(f1),
      f2(0),
      formulaF1(formulaF1),
      formulaF2("0"),
      radius1(radius1),
      radius2(radius2),
      formulaRadius1(formulaRadius1),
      formulaRadius2(formulaRadius2),
      center(center),
      isFlipped(false),
      formulaLength(formulaLength),
      rotationAngle(rotationAngle)
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::VEllipticalArcData(const VPointF &center, qreal radius1, qreal radius2, qreal f1,
                                       qreal rotationAngle)
    : f1(f1),
      f2(0),
      formulaF1(QString().number(f1)),
      formulaF2("0"),
      radius1(radius1),
      radius2(radius2),
      formulaRadius1(QString().number(qApp->fromPixel(radius1))),
      formulaRadius2(QString().number(qApp->fromPixel(radius2))),
      center(center),
      isFlipped(false),
      formulaLength(),
      rotationAngle(rotationAngle)
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::VEllipticalArcData(const VEllipticalArcData &arc)
    : QSharedData(arc),
      f1(arc.f1),
      f2(arc.f2),
      formulaF1(arc.formulaF1),
      formulaF2(arc.formulaF2),
      radius1(arc.radius1),
      radius2(arc.radius2),
      formulaRadius1(arc.formulaRadius1),
      formulaRadius2(arc.formulaRadius2),
      center(arc.center),
      isFlipped(arc.isFlipped),
      formulaLength(arc.formulaLength),
      rotationAngle(arc.rotationAngle)
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::~VEllipticalArcData()
{}

#ifdef Q_CC_GNU
#pragma GCC diagnostic pop
#endif

#endif // VELLIPTICALARC_P


#ifndef VELLIPTICALARC_P
#define VELLIPTICALARC_P

#include <QSharedData>
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/diagnostic.h"
#include "vpointf.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

class VEllipticalArcData : public QSharedData
{
public:
    VEllipticalArcData ();
    VEllipticalArcData (qreal radius1, qreal radius2, const QString &formulaRadius1, const QString &formulaRadius2,
                        qreal rotationAngle);
    VEllipticalArcData(qreal radius1, qreal radius2, qreal rotationAngle);
    VEllipticalArcData(const VEllipticalArcData &arc);

    virtual ~VEllipticalArcData();

    /** @brief radius1 elliptical arc major radius. */
    qreal   radius1;
    /** @brief radius2 elliptical arc minor radius. */
    qreal   radius2;
    /** @brief formulaRadius1 formula for elliptical arc major radius. */
    QString formulaRadius1;
    /** @brief formulaRadius2 formula for elliptical arc minor radius. */
    QString formulaRadius2;
    /** @brief rotationAngle in degree. */
    qreal   rotationAngle;

private:
    VEllipticalArcData &operator=(const VEllipticalArcData &) Q_DECL_EQ_DELETE;
};

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::VEllipticalArcData()
    : radius1(0),
      radius2(0),
      formulaRadius1(),
      formulaRadius2(),
      rotationAngle(0)
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::VEllipticalArcData(qreal radius1, qreal radius2, const QString &formulaRadius1,
                                       const QString &formulaRadius2, qreal rotationAngle)
    : radius1(radius1),
      radius2(radius2),
      formulaRadius1(formulaRadius1),
      formulaRadius2(formulaRadius2),
      rotationAngle(rotationAngle)
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::VEllipticalArcData(qreal radius1, qreal radius2, qreal rotationAngle)
    : radius1(radius1),
      radius2(radius2),
      formulaRadius1(QString().number(qApp->fromPixel(radius1))),
      formulaRadius2(QString().number(qApp->fromPixel(radius2))),
      rotationAngle(rotationAngle)
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::VEllipticalArcData(const VEllipticalArcData &arc)
    : QSharedData(arc),
      radius1(arc.radius1),
      radius2(arc.radius2),
      formulaRadius1(arc.formulaRadius1),
      formulaRadius2(arc.formulaRadius2),
      rotationAngle(arc.rotationAngle)
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::~VEllipticalArcData()
{}

QT_WARNING_POP

#endif // VELLIPTICALARC_P


/************************************************************************
 **
 **  @file   vpatternrecipe.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 7, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/
#ifndef VPATTERNRECIPE_H
#define VPATTERNRECIPE_H

#include "../ifc/xml/vdomdocument.h"

class VContainer;
class VAbstractPattern;
class VMeasurement;
class VIncrement;
class VToolRecord;
class VFormula;

class VPatternRecipe : public VDomDocument
{
    Q_DECLARE_TR_FUNCTIONS(VPatternRecipe)
public:
    VPatternRecipe(VContainer *data, VAbstractPattern *pattern, QObject *parent = nullptr);
    virtual ~VPatternRecipe() Q_DECL_EQ_DEFAULT;

private:
    Q_DISABLE_COPY(VPatternRecipe)

    VContainer *m_data;
    VAbstractPattern *m_pattern;

    QDomElement Prerequisite();

    QDomElement Measurements();
    QDomElement Measurement(const QSharedPointer<VMeasurement> &m);

    QDomElement Increments();
    QDomElement PreviewCalculations();
    QDomElement Increment(const QSharedPointer<VIncrement> &incr);

    QDomElement Content();

    QDomElement Draft(const QDomElement &draft);
    QDomElement Step(const VToolRecord &tool);

    QDomElement BasePoint(const VToolRecord &record);
    QDomElement EndLine(const VToolRecord &record);
    QDomElement Line(const VToolRecord &record);
    QDomElement AlongLine(const VToolRecord &record);
    QDomElement ShoulderPoint(const VToolRecord &record);
    QDomElement Normal(const VToolRecord &record);
    QDomElement Bisector(const VToolRecord &record);
    QDomElement LineIntersect(const VToolRecord &record);
    QDomElement Spline(const VToolRecord &record);
    QDomElement CubicBezier(const VToolRecord &record);
    QDomElement Arc(const VToolRecord &record);
    QDomElement ArcWithLength(const VToolRecord &record);
    QDomElement SplinePath(const VToolRecord &record);
    QDomElement CubicBezierPath(const VToolRecord &record);
    QDomElement PointOfContact(const VToolRecord &record);
    QDomElement Height(const VToolRecord &record);
    QDomElement Triangle(const VToolRecord &record);
    QDomElement PointOfIntersection(const VToolRecord &record);
    QDomElement CutArc(const VToolRecord &record);
    QDomElement CutSpline(const VToolRecord &record);
    QDomElement CutSplinePath(const VToolRecord &record);
    QDomElement LineIntersectAxis(const VToolRecord &record);
    QDomElement CurveIntersectAxis(const VToolRecord &record);
    QDomElement PointOfIntersectionArcs(const VToolRecord &record);
    QDomElement PointOfIntersectionCircles(const VToolRecord &record);
    QDomElement PointOfIntersectionCurves(const VToolRecord &record);
    QDomElement PointFromCircleAndTangent(const VToolRecord &record);
    QDomElement PointFromArcAndTangent(const VToolRecord &record);
    QDomElement TrueDarts(const VToolRecord &record);
    QDomElement EllipticalArc(const VToolRecord &record);

    void Formula(QDomElement &step, const VFormula &formula, const QString &formulaStr, const QString &formulaValue);

    template <typename T>
    void ToolAttributes(QDomElement &step, T* tool);

    template <typename T>
    void LineAttributes(QDomElement &step, T* tool);

    template <typename T>
    void CurveAttributes(QDomElement &step, T* tool);
};

#endif // VPATTERNRECIPE_H

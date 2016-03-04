/************************************************************************
 **
 **  @file   vistoolcutsplinepath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 9, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "vistoolcutsplinepath.h"
#include "../../vpatterndb/vcontainer.h"
#include "../../vgeometry/vsplinepath.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolCutSplinePath::VisToolCutSplinePath(const VContainer *data, QGraphicsItem *parent)
    :VisPath(data, parent), point(nullptr), splPath1(nullptr), splPath2(nullptr), length(0)
{
    splPath1 = InitItem<QGraphicsPathItem>(Qt::darkGreen, this);
    splPath1->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
    splPath2 = InitItem<QGraphicsPathItem>(Qt::darkRed, this);
    splPath2->setFlag(QGraphicsItem::ItemStacksBehindParent, false);

    point = InitPoint(mainColor, this);
    point->setZValue(2);
    point->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolCutSplinePath::~VisToolCutSplinePath()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCutSplinePath::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VSplinePath> splPath = Visualization::data->GeometricObject<VSplinePath>(object1Id);
        DrawPath(this, splPath->GetPath(PathDirection::Show), supportColor, Qt::SolidLine, Qt::RoundCap);

        if (qFuzzyCompare(1 + length, 1 + 0) == false)
        {
            QPointF spl1p2, spl1p3, spl2p2, spl2p3;
            qint32 p1 = 0, p2 = 0;

            // TODO make refactoring. CutSplPath repeat twice. Here and in VToolCutSpline.
            const QPointF cutPoint = splPath->CutSplinePath(length, p1, p2, spl1p2, spl1p3, spl2p2, spl2p3);
            VPointF p = VPointF(cutPoint);

            VSplinePoint splP1 = splPath->at(p1);
            VSplinePoint splP2 = splPath->at(p2);
            const VSpline spl1 = VSpline(splP1.P(), spl1p2, spl1p3, p);
            const VSpline spl2 = VSpline(p, spl2p2, spl2p3, splP2.P());

            VSplinePath spPath1 = VSplinePath();
            VSplinePath spPath2 = VSplinePath();

            for (qint32 i = 0; i < splPath->CountPoint(); i++)
            {
                if (i <= p1 && i < p2)
                {
                    if (i == p1)
                    {
                        const qreal angle1 = spl1.GetStartAngle()+180;
                        const QString angle1F = QString().number(angle1);

                        spPath1.append(VSplinePoint(splP1.P(), angle1, angle1F, spl1.GetStartAngle(),
                                                    spl1.GetStartAngleFormula(), splP1.Length1(),
                                                    splP1.Length1Formula(), spl1.GetC1Length(),
                                                    spl1.GetC1LengthFormula()));

                        const qreal angle2 = spl1.GetEndAngle()+180;
                        const QString angle2F = QString().number(angle2);

                        const auto cutPoint = VSplinePoint(p, spl1.GetEndAngle(), spl1.GetEndAngleFormula(), angle2,
                                                           angle2F, spl1.GetC2Length(), spl1.GetC2LengthFormula(),
                                                           spl2.GetC1Length(), spl2.GetC1LengthFormula());

                        spPath1.append(cutPoint);
                        continue;
                    }
                    spPath1.append(splPath->at(i));
                }
                else
                {
                    if (i == p2)
                    {
                        const qreal angle1 = spl2.GetStartAngle()+180;
                        const QString angle1F = QString().number(angle1);

                        const auto cutPoint = VSplinePoint(p, angle1, angle1F, spl2.GetStartAngle(),
                                                           spl2.GetStartAngleFormula(), spl1.GetC2Length(),
                                                           spl1.GetC2LengthFormula(), spl2.GetC1Length(),
                                                           spl2.GetC1LengthFormula());

                        spPath2.append(cutPoint);

                        const qreal angle2 = spl2.GetEndAngle()+180;
                        const QString angle2F = QString().number(angle2);

                        spPath2.append(VSplinePoint(splP2.P(), spl2.GetEndAngle(), spl2.GetEndAngleFormula(), angle2,
                                                    angle2F, spl2.GetC2Length(), spl2.GetC2LengthFormula(),
                                                    splP2.Length2(), splP2.Length2Formula()));

                        continue;
                    }
                    spPath2.append(splPath->at(i));
                }
            }

            DrawPoint(point, cutPoint, mainColor);

            DrawPath(splPath1, spPath1.GetPath(PathDirection::Show), Qt::darkGreen, Qt::SolidLine, Qt::RoundCap);
            DrawPath(splPath2, spPath2.GetPath(PathDirection::Show), Qt::darkRed, Qt::SolidLine, Qt::RoundCap);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCutSplinePath::setLength(const QString &expression)
{
    length = FindLength(expression, Visualization::data->PlainVariables());
}

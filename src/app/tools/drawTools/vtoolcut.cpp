/************************************************************************
 **
 **  @file   vtoolcut.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "vtoolcut.h"
#include "../../geometry/vpointf.h"

//---------------------------------------------------------------------------------------------------------------------
VToolCut::VToolCut(VPattern *doc, VContainer *data, const quint32 &id, const QString &formula,
                   const quint32 &curveCutId, const quint32 &curve1id, const quint32 &curve2id,
                   QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), formula(formula), firstCurve(nullptr), secondCurve(nullptr),
      curveCutId(curveCutId), curve1id(curve1id), curve2id(curve2id)
{
    Q_ASSERT_X(curveCutId > 0, Q_FUNC_INFO, "curveCutId <= 0");
    Q_ASSERT_X(curve1id > 0, Q_FUNC_INFO, "curve1id <= 0");
    Q_ASSERT_X(curve2id > 0, Q_FUNC_INFO, "curve2id <= 0");

    firstCurve = new VSimpleCurve(curve1id, &currentColor, SimpleCurvePoint::ForthPoint, &factor);
    firstCurve->setParentItem(this);
    connect(firstCurve, &VSimpleCurve::Choosed, this, &VToolCut::CurveChoosed);
    connect(firstCurve, &VSimpleCurve::HoverPath, this, &VToolCut::HoverPath);

    secondCurve = new VSimpleCurve(curve2id, &currentColor, SimpleCurvePoint::FirstPoint, &factor);
    secondCurve->setParentItem(this);
    connect(secondCurve, &VSimpleCurve::Choosed, this, &VToolCut::CurveChoosed);
    connect(secondCurve, &VSimpleCurve::HoverPath, this, &VToolCut::HoverPath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
 * @param newName new name active pattern peace.
 */
void VToolCut::ChangedActivDraw(const QString &newName)
{
    bool flag = true;
    if (nameActivDraw == newName)
    {
        currentColor = Qt::black;
        flag = true;
    }
    else
    {
        currentColor = Qt::gray;
        flag = false;
    }
    firstCurve->ChangedActivDraw(flag);
    secondCurve->ChangedActivDraw(flag);
    VToolPoint::ChangedActivDraw(newName);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCut::HoverPath(quint32 id, SimpleCurvePoint curvePosition, PathDirection direction)
{
    VSimpleCurve* simpleCurve = qobject_cast<VSimpleCurve*>(sender());
    if (simpleCurve)
    {
        RefreshCurve(simpleCurve, id, curvePosition, direction);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry  refresh item on scene.
 */
void VToolCut::RefreshGeometry()
{
    RefreshCurve(firstCurve, curve1id, SimpleCurvePoint::ForthPoint);
    RefreshCurve(secondCurve, curve2id, SimpleCurvePoint::FirstPoint);
    VToolPoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<const VPointF *>(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement referens value for used objects.
 */
void VToolCut::RemoveReferens()
{
    doc->DecrementReferens(curveCutId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCut::FullUpdateCurveFromFile(const QString &attrCurve)
{
    Q_ASSERT_X(attrCurve.isEmpty() == false, Q_FUNC_INFO, "attribute name is empty");

    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        formula = domElement.attribute(AttrLength, "");
        curveCutId = domElement.attribute(attrCurve, "").toUInt();
    }
    RefreshGeometry();
}

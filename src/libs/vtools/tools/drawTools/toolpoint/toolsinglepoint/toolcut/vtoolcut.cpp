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

#include "vtoolcut.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vformula.h"

//---------------------------------------------------------------------------------------------------------------------
VToolCut::VToolCut(VAbstractPattern *doc, VContainer *data, const quint32 &id, const QString &formula,
                   const quint32 &curveCutId, const QString &color, QGraphicsItem *parent)
    :VToolSinglePoint(doc, data, id, parent), formula(formula), curveCutId(curveCutId), detailsMode(false)
{
    Q_ASSERT_X(curveCutId != 0, Q_FUNC_INFO, "curveCutId == 0"); //-V654 //-V712

    lineColor = color;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCut::Disable(bool disable, const QString &namePP)
{
    VToolSinglePoint::Disable(disable, namePP);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCut::DetailsMode(bool mode)
{
    detailsMode = mode;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolCut::FullUpdateFromFile()
{
    ReadAttributes();
    RefreshGeometry();
    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VToolCut::getCurveCutId() const
{
    return curveCutId;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void VToolCut::setCurveCutId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        curveCutId = value;
        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolCut::GetFormula() const
{
    VFormula val(formula, getData());
    val.setCheckZero(true);
    val.setToolId(id);
    val.setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit()));
    return val;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCut::SetFormula(const VFormula &value)
{
    if (value.error() == false)
    {
        formula = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry  refresh item on scene.
 */
void VToolCut::RefreshGeometry()
{
    VToolSinglePoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<VPointF>(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement referens value for used objects.
 */
void VToolCut::RemoveReferens()
{
    const auto curve = VAbstractTool::data.GetGObject(curveCutId);

    doc->DecrementReferens(curve->getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void VToolCut::FullUpdateCurveFromFile(const QString &attrCurve)
{
    Q_ASSERT_X(not attrCurve.isEmpty(), Q_FUNC_INFO, "attribute name is empty");

    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        formula = domElement.attribute(AttrLength, "");
        curveCutId = domElement.attribute(attrCurve, "").toUInt();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCut::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolSinglePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrColor, lineColor);
}

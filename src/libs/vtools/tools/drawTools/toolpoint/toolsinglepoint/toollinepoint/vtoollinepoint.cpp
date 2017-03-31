/************************************************************************
 **
 **  @file   vtoollinepoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "vtoollinepoint.h"

#include <QColor>
#include <QGraphicsLineItem>
#include <QLineF>
#include <QPen>
#include <QPoint>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/xml/vdomdocument.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../../../../vabstracttool.h"
#include "../../../vdrawtool.h"
#include "../vtoolsinglepoint.h"

template <class T> class QSharedPointer;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolLinePoint constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeLine line type.
 * @param lineColor line color.
 * @param formula string with length formula.
 * @param basePointId id base line point.
 * @param angle line angle.
 * @param parent parent object.
 */
VToolLinePoint::VToolLinePoint(VAbstractPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                               const QString &lineColor, const QString &formula, const quint32 &basePointId,
                               const qreal &angle, QGraphicsItem *parent)
    :VToolSinglePoint(doc, data, id, parent), formulaLength(formula), angle(angle), basePointId(basePointId),
      mainLine(nullptr), lineColor(lineColor)
{
    this->typeLine = typeLine;
    Q_ASSERT_X(basePointId != 0, Q_FUNC_INFO, "basePointId == 0"); //-V654 //-V712
    QPointF point1 = static_cast<QPointF>(*data->GeometricObject<VPointF>(basePointId));
    QPointF point2 = static_cast<QPointF>(*data->GeometricObject<VPointF>(id));
    mainLine = new QGraphicsLineItem(QLineF(point1 - point2, QPointF()), this);
    mainLine->setPen(QPen(CorrectColor(lineColor),
                          qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor,
                          LineStyleToPenStyle(typeLine)));
    mainLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
}

//---------------------------------------------------------------------------------------------------------------------
VToolLinePoint::~VToolLinePoint()
{
    delete mainLine;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry  refresh item on scene.
 */
void VToolLinePoint::RefreshGeometry()
{
    mainLine->setPen(QPen(CorrectColor(lineColor),
                          qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor,
                          LineStyleToPenStyle(typeLine)));
    VToolSinglePoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<VPointF>(id));
    QPointF point = static_cast<QPointF>(*VDrawTool::data.GeometricObject<VPointF>(id));
    QPointF basePoint = static_cast<QPointF>(*VDrawTool::data.GeometricObject<VPointF>(basePointId));
    mainLine->setLine(QLineF(basePoint - point, QPointF()));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolLinePoint::RemoveReferens()
{
    const auto basePoint = VAbstractTool::data.GetGObject(basePointId);
    doc->DecrementReferens(basePoint->getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLinePoint::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolSinglePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrTypeLine, typeLine);
    doc->SetAttribute(tag, AttrLineColor, lineColor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolLinePoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLinePoint::Disable(bool disable, const QString &namePP)
{
    VToolSinglePoint::Disable(disable, namePP);
    mainLine->setPen(QPen(CorrectColor(lineColor),
                          qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor,
                          LineStyleToPenStyle(typeLine)));
    mainLine->setEnabled(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolLinePoint::FullUpdateFromFile()
{
    ReadAttributes();
    RefreshGeometry();
    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
qreal VToolLinePoint::GetAngle() const
{
    return angle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLinePoint::SetAngle(const qreal &value)
{
    angle = value;
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolLinePoint::GetLineColor() const
{
    return lineColor;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLinePoint::SetLineColor(const QString &value)
{
    lineColor = value;

    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolLinePoint::GetBasePointId() const
{
    return basePointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLinePoint::SetBasePointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        basePointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolLinePoint::GetFormulaLength() const
{
    VFormula fLength(formulaLength, this->getData());
    fLength.setCheckZero(true);
    fLength.setToolId(id);
    fLength.setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit()));

    return fLength;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLinePoint::SetFormulaLength(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaLength = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolLinePoint::BasePointName() const
{
    return VAbstractTool::data.GetGObject(basePointId)->name();
}

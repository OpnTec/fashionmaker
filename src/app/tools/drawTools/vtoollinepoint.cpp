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
 **  Copyright (C) 2013 Valentina project
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
#include "../../geometry/vpointf.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolLinePoint constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeLine line type.
 * @param formulaLength string with length formula.
 * @param basePointId id base line point.
 * @param angle line angle.
 * @param parent parent object.
 */
VToolLinePoint::VToolLinePoint(VPattern *doc, VContainer *data, const quint32 &id,
                               const QString &typeLine, const QString &formula, const quint32 &basePointId,
                               const qreal &angle, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent),  formulaLength(formula), angle(angle), basePointId(basePointId),
      mainLine(nullptr)
{
    this->typeLine = typeLine;
    Q_ASSERT_X(basePointId > 0, Q_FUNC_INFO, "basePointId <= 0");
    QPointF point1 = data->GeometricObject<VPointF>(basePointId)->toQPointF();
    QPointF point2 = data->GeometricObject<VPointF>(id)->toQPointF();
    mainLine = new QGraphicsLineItem(QLineF(point1 - point2, QPointF()), this);
    mainLine->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())/factor, LineStyle(typeLine)));
    mainLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
 * @param newName new name active pattern peace.
 */
void VToolLinePoint::ChangedActivDraw(const QString &newName)
{
    if (nameActivDraw == newName)
    {
        currentColor = Qt::black;
    }
    else
    {
        currentColor = Qt::gray;
    }
    mainLine->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor, LineStyle(typeLine)));
    VToolPoint::ChangedActivDraw(newName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry  refresh item on scene.
 */
void VToolLinePoint::RefreshGeometry()
{
    mainLine->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor, LineStyle(typeLine)));
    VToolPoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<VPointF>(id));
    QPointF point = VDrawTool::data.GeometricObject<VPointF>(id)->toQPointF();
    QPointF basePoint = VDrawTool::data.GeometricObject<VPointF>(basePointId)->toQPointF();
    mainLine->setLine(QLineF(basePoint - point, QPointF()));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolLinePoint::RemoveReferens()
{
    doc->DecrementReferens(basePointId);
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
qreal VToolLinePoint::getAngle() const
{
    return angle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLinePoint::setAngle(const qreal &value)
{
    angle = value;
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolLinePoint::getBasePointId() const
{
    return basePointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLinePoint::setBasePointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        basePointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolLinePoint::getFormulaLength() const
{
    VFormula fLength(formulaLength, this->getData());
    fLength.setCheckZero(true);
    fLength.setToolId(id);
    fLength.setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit()));

    return fLength;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLinePoint::setFormulaLength(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaLength = value.getFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vpiecenode.h"
#include "vpiecenode_p.h"
#include "vcontainer.h"
#include "calculator.h"

#include <QDataStream>
#include <QtNumeric>

namespace
{
//---------------------------------------------------------------------------------------------------------------------
qreal EvalFormula(const VContainer *data, QString formula)
{
    if (formula.isEmpty())
    {
        return -1;
    }
    else
    {
        try
        {
            // Replace line return character with spaces for calc if exist
            formula.replace("\n", " ");
            QScopedPointer<Calculator> cal(new Calculator());
            const qreal result = cal->EvalFormula(data->PlainVariables(), formula);

            if (qIsInf(result) || qIsNaN(result))
            {
                return -1;
            }
            return result;
        }
        catch (qmu::QmuParserError &e)
        {
            Q_UNUSED(e)
            return -1;
        }
    }
}
}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode::VPieceNode()
    : d(new VPieceNodeData)
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode::VPieceNode(quint32 id, Tool typeTool, bool reverse)
    : d(new VPieceNodeData(id, typeTool, reverse))
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode::VPieceNode(const VPieceNode &node)
    : d (node.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode &VPieceNode::operator=(const VPieceNode &node)
{
    if ( &node == this )
    {
        return *this;
    }
    d = node.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode::~VPieceNode()
{}

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &out, const VPieceNode &p)
{
    out << p.d;
    return out;
}

//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &in, VPieceNode &p)
{
    in >> *p.d;
    return in;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VPieceNode::GetId() const
{
    return d->m_id;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetId(quint32 id)
{
    d->m_id = id;
}

//---------------------------------------------------------------------------------------------------------------------
Tool VPieceNode::GetTypeTool() const
{
    return d->m_typeTool;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetTypeTool(Tool value)
{
    d->m_typeTool = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::GetReverse() const
{
    return d->m_reverse;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetReverse(bool reverse)
{
    if (d->m_typeTool != Tool::NodePoint)
    {
        d->m_reverse = reverse;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPieceNode::GetSABefore(const VContainer *data) const
{
    if (d->m_formulaWidthBefore == currentSeamAllowance)
    {
        return -1;
    }

    return EvalFormula(data, d->m_formulaWidthBefore);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPieceNode::GetSABefore(const VContainer *data, Unit unit) const
{
    if (d->m_formulaWidthBefore == currentSeamAllowance)
    {
        return -1;
    }

    qreal value = EvalFormula(data, d->m_formulaWidthBefore);
    if (value >= 0)
    {
        value = ToPixel(value, unit);
    }
    return value;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPieceNode::GetFormulaSABefore() const
{
    return d->m_formulaWidthBefore;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetFormulaSABefore(const QString &formula)
{
    if (d->m_typeTool == Tool::NodePoint)
    {
        d->m_formulaWidthBefore = formula;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPieceNode::GetSAAfter(const VContainer *data) const
{
    if (d->m_formulaWidthAfter == currentSeamAllowance)
    {
        return -1;
    }

    return EvalFormula(data, d->m_formulaWidthAfter);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPieceNode::GetSAAfter(const VContainer *data, Unit unit) const
{
    if (d->m_formulaWidthAfter == currentSeamAllowance)
    {
        return -1;
    }

    qreal value = EvalFormula(data, d->m_formulaWidthAfter);
    if (value >= 0)
    {
        value = ToPixel(value, unit);
    }
    return value;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPieceNode::GetFormulaSAAfter() const
{
    return d->m_formulaWidthAfter;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetFormulaSAAfter(const QString &formula)
{
    if (d->m_typeTool == Tool::NodePoint)
    {
        d->m_formulaWidthAfter = formula;
    }
}

//---------------------------------------------------------------------------------------------------------------------
PieceNodeAngle VPieceNode::GetAngleType() const
{
    return d->m_angleType;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetAngleType(PieceNodeAngle type)
{
    if (d->m_typeTool == Tool::NodePoint)
    {
        d->m_angleType = type;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::IsPassmark() const
{
    return d->m_isPassmark;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetPassmark(bool passmark)
{
    if (GetTypeTool() == Tool::NodePoint)
    {
        d->m_isPassmark = passmark;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::IsMainPathNode() const
{
    return d->m_isMainPathNode;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetMainPathNode(bool value)
{
    d->m_isMainPathNode = value;
}

//---------------------------------------------------------------------------------------------------------------------
PassmarkLineType VPieceNode::GetPassmarkLineType() const
{
    return d->m_passmarkLineType;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetPassmarkLineType(PassmarkLineType lineType)
{
    d->m_passmarkLineType = lineType;
}

//---------------------------------------------------------------------------------------------------------------------
PassmarkAngleType VPieceNode::GetPassmarkAngleType() const
{
    return d->m_passmarkAngleType;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetPassmarkAngleType(PassmarkAngleType angleType)
{
    d->m_passmarkAngleType = angleType;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::IsShowSecondPassmark() const
{
    return d->m_isShowSecondPassmark;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetShowSecondPassmark(bool value)
{
    d->m_isShowSecondPassmark = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::IsExcluded() const
{
    return d->m_excluded;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetExcluded(bool exclude)
{
    d->m_excluded = exclude;
}

/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#ifndef VPIECENODE_H
#define VPIECENODE_H

#include <QtGlobal>
#include <QSharedDataPointer>
#include <QMetaType>

#include "../vmisc/def.h"

class VPieceNodeData;
class VContainer;

class VPieceNode
{
public:
    VPieceNode();
    VPieceNode(quint32 id, Tool typeTool, bool reverse = false);
    VPieceNode(const VPieceNode &node);

    ~VPieceNode();

    VPieceNode &operator=(const VPieceNode &node);
#ifdef Q_COMPILER_RVALUE_REFS
    VPieceNode(const VPieceNode &&node) Q_DECL_NOTHROW;
    VPieceNode &operator=(VPieceNode &&node) Q_DECL_NOTHROW;
#endif

    friend QDataStream& operator<<(QDataStream& out, const VPieceNode& p);
    friend QDataStream& operator>>(QDataStream& in, VPieceNode& p);

    quint32 GetId() const;
    void    SetId(quint32 id);

    Tool GetTypeTool() const;
    void SetTypeTool(Tool value);

    bool GetReverse() const;
    void SetReverse(bool reverse);

    bool IsExcluded() const;
    void SetExcluded(bool exclude);

    qreal GetSABefore(const VContainer *data) const;
    qreal GetSABefore(const VContainer *data, Unit unit) const;

    QString GetFormulaSABefore() const;
    void    SetFormulaSABefore(const QString &formula);

    qreal GetSAAfter(const VContainer *data) const;
    qreal GetSAAfter(const VContainer *data, Unit unit) const;

    QString GetFormulaSAAfter() const;
    void    SetFormulaSAAfter(const QString &formula);

    QString GetFormulaPassmarkLength() const;
    void    SetFormulaPassmarkLength(const QString &formula);

    qreal GetPassmarkLength(const VContainer *data, Unit unit) const;

    PieceNodeAngle GetAngleType() const;
    void           SetAngleType(PieceNodeAngle type);

    bool IsPassmark() const;
    void SetPassmark(bool passmark);

    bool IsMainPathNode() const;
    void SetMainPathNode(bool value);

    PassmarkLineType GetPassmarkLineType() const;
    void             SetPassmarkLineType(PassmarkLineType lineType);

    PassmarkAngleType GetPassmarkAngleType() const;
    void              SetPassmarkAngleType(PassmarkAngleType angleType);

    bool IsShowSecondPassmark() const;
    void SetShowSecondPassmark(bool value);

    bool IsCheckUniqueness() const;
    void SetCheckUniqueness(bool value);

    bool IsManualPassmarkLength() const;
    void SetManualPassmarkLength(bool value);
private:
    QSharedDataPointer<VPieceNodeData> d;
};

Q_DECLARE_METATYPE(VPieceNode)
Q_DECLARE_TYPEINFO(VPieceNode, Q_MOVABLE_TYPE);

#endif // VPIECENODE_H

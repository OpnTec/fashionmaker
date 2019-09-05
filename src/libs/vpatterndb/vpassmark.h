/************************************************************************
 **
 **  @file   vpassmark.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 5, 2019
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
#ifndef VPASSMARK_H
#define VPASSMARK_H

#include <QtGlobal>
#include <QMetaType>

#include "vpiece.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vmisc/typedef.h"

enum class PassmarkStatus: qint8
{
    Error = 0,
    Common = 1,
    Rollback = -1
};

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

struct VPiecePassmarkData
{
    VSAPoint previousSAPoint{};
    VSAPoint passmarkSAPoint{};
    VSAPoint nextSAPoint{};
    qreal saWidth{0};
    QString nodeName{};
    QString pieceName{};
    PassmarkLineType passmarkLineType{PassmarkLineType::OneLine};
    PassmarkAngleType passmarkAngleType{PassmarkAngleType::Straightforward};
    bool isMainPathNode{true};
    bool isShowSecondPassmark{true};
    int passmarkIndex{-1};
    vidtype id{NULL_ID};

    QJsonObject toJson() const;
};

Q_DECLARE_METATYPE(VPiecePassmarkData)
Q_DECLARE_TYPEINFO(VPiecePassmarkData, Q_MOVABLE_TYPE);

QT_WARNING_POP

enum class PassmarkSide : char { All=0, Left=1, Right=2 };

class VPassmark
{
public:
    VPassmark();
    explicit VPassmark(const VPiecePassmarkData &data);

    QVector<QLineF> FullPassmark(const VPiece& piece, const VContainer *data) const;
    QVector<QLineF> SAPassmark(const VPiece& piece, const VContainer *data, PassmarkSide side) const;
    QVector<QLineF> SAPassmark(const QVector<QPointF> &seamAllowance, PassmarkSide side) const;
    QVector<QLineF> BuiltInSAPassmark(const VPiece &piece, const VContainer *data) const;

    QVector<QLineF> BuiltInSAPassmarkBaseLine(const VPiece &piece) const;
    QVector<QLineF> SAPassmarkBaseLine(const VPiece &piece, const VContainer *data, PassmarkSide side) const;
    QVector<QLineF> SAPassmarkBaseLine(const QVector<QPointF> &seamAllowance, PassmarkSide side) const;

    QPainterPath SAPassmarkPath(const VPiece& piece, const VContainer *data, PassmarkSide side) const;
    QPainterPath BuiltInSAPassmarkPath(const VPiece &piece, const VContainer *data) const;

    bool IsNull() const;

    VPiecePassmarkData Data() const;

    static QLineF FindIntersection(const QLineF &line, const QVector<QPointF> &seamAllowance);

    static const qreal passmarkRadiusFactor;
private:
    VPiecePassmarkData m_data{};
    bool m_null{true};

    QVector<QLineF> MakeSAPassmark(const QVector<QPointF> &seamAllowance, PassmarkSide side) const;

};

#endif // VPASSMARK_H

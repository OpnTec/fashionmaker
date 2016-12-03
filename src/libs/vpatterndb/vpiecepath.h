/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 11, 2016
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

#ifndef VPIECEPATH_H
#define VPIECEPATH_H

#include <QtGlobal>
#include <QSharedDataPointer>

#include "../vmisc/def.h"

class VPiecePathData;
class VPieceNode;
class QPointF;
class VPointF;
class VContainer;
class VSAPoint;
class QPainterPath;
class VAbstractCurve;

class VPiecePath
{
public:
    VPiecePath();
    explicit VPiecePath(PiecePathType type);
    VPiecePath(const VPiecePath &path);
    VPiecePath &operator=(const VPiecePath &path);
    ~VPiecePath();

    void   Append(const VPieceNode &node);
    void   Clear();
    qint32 CountNodes() const;

    VPieceNode & operator[](int indx);
    const VPieceNode & at ( int indx ) const;

    QVector<VPieceNode> GetNodes() const;
    void                SetNodes(const QVector<VPieceNode> &nodes);

    PiecePathType GetType() const;
    void          SetType(PiecePathType type);

    QString GetName() const;
    void    SetName(const QString &name);

    Qt::PenStyle GetPenType() const;
    void         SetPenType(const Qt::PenStyle &type);

    QVector<QPointF>  PathPoints(const VContainer *data) const;
    QVector<VPointF>  PathNodePoints(const VContainer *data) const;
    QVector<VSAPoint> SeamAllowancePoints(const VContainer *data, qreal width, bool reverse) const;

    QPainterPath PainterPath(const VContainer *data) const;

    QVector<quint32> MissingNodes(const VPiecePath &path) const;

    int indexOfNode(quint32 id) const;

    VSAPoint StartSegment(const VContainer *data, int i, bool reverse) const;
    VSAPoint EndSegment(const VContainer *data, int i, bool reverse) const;

    QPointF NodePreviousPoint(const VContainer *data, int i) const;
    QPointF NodeNextPoint(const VContainer *data, int i) const;

    static VSAPoint StartSegment(const VContainer *data, const QVector<VPieceNode> &nodes, int i, bool reverse);
    static VSAPoint EndSegment(const VContainer *data, const QVector<VPieceNode> &nodes, int i, bool reverse);

    static VSAPoint PreparePointEkv(const VPieceNode &node, const VContainer *data);

    static QVector<VSAPoint> CurveSeamAllowanceSegment(const VContainer *data, const QVector<VPieceNode> &nodes,
                                                       const QSharedPointer<VAbstractCurve> &curve,
                                                       int i, bool reverse, qreal width);

private:
    QSharedDataPointer<VPiecePathData> d;

    static VSAPoint CurvePoint(const VSAPoint &candidate, const VContainer *data, const VPieceNode &node,
                               const QSharedPointer<VAbstractCurve> &curve);
};

Q_DECLARE_TYPEINFO(VPiecePath, Q_MOVABLE_TYPE);

#endif // VPIECEPATH_H

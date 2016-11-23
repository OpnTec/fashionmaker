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

#ifndef VPIECE_H
#define VPIECE_H

#include <QtGlobal>
#include <QSharedDataPointer>

#include "../vlayout/vabstractpiece.h"

class QPainterPath;
class VPieceData;
class VPieceNode;
class QPointF;
class VContainer;
template <class T> class QVector;
template <class T>class QSharedPointer;
class VAbstractCurve;
class VPiecePath;

class VPiece : public VAbstractPiece
{
public:
    VPiece();
    VPiece(const VPiece &piece);
    VPiece &operator=(const VPiece &piece);
    virtual ~VPiece();

    VPiecePath GetPath() const;
    VPiecePath &GetPath();
    void       SetPath(const VPiecePath &path);

    QVector<QPointF> MainPathPoints(const VContainer *data) const;
    QVector<QPointF> MainPathNodePoints(const VContainer *data) const;
    QVector<QPointF> SeamAllowancePoints(const VContainer *data) const;

    QPainterPath MainPathPath(const VContainer *data) const;
    QPainterPath SeamAllowancePath(const VContainer *data) const;

    qreal GetMx() const;
    void  SetMx(qreal value);

    qreal GetMy() const;
    void  SetMy(qreal value);

    bool IsInLayout() const;
    void SetInLayout(bool inLayout);

    QVector<VPieceNode> Missing(const VPiece &det) const;

    int indexOfNode(const quint32 &id) const;

private:
    QSharedDataPointer<VPieceData> d;

    void CurveSeamAllowanceSegment(QVector<VSAPoint> &pointsEkv, const VContainer *data,
                                   const QSharedPointer<VAbstractCurve> &curve, int i, bool reverse) const;

    static int indexOfNode(const QVector<VPieceNode> &list, quint32 id);
};

Q_DECLARE_TYPEINFO(VPiece, Q_MOVABLE_TYPE);

#endif // VPIECE_H

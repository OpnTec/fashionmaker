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

class VPieceData;
class VPieceNode;

class VPiece
{
public:
    VPiece();
    VPiece(const VPiece &piece);
    VPiece &operator=(const VPiece &piece);
    virtual ~VPiece();

    void   Append(const VPieceNode &node);
    void   Clear();
    void   ClearNodes();
    qint32 CountNode() const;

    VPieceNode & operator[](int indx);
    const VPieceNode & at ( int indx ) const;

    QVector<VPieceNode> GetNodes() const;
    void                SetNodes(const QVector<VPieceNode> &nodes);
private:
    QSharedDataPointer<VPieceData> d;
};

Q_DECLARE_TYPEINFO(VPiece, Q_MOVABLE_TYPE);

#endif // VPIECE_H

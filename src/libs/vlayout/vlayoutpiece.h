/************************************************************************
 **
 **  @file   vlayoutdetail.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef VLAYOUTDETAIL_H
#define VLAYOUTDETAIL_H

#include <qcompilerdetection.h>
#include <QDate>
#include <QLineF>
#include <QMatrix>
#include <QPointF>
#include <QRectF>
#include <QSharedDataPointer>
#include <QString>
#include <QTypeInfo>
#include <QVector>
#include <QtGlobal>
#include <QCoreApplication>

#include "vabstractpiece.h"
#include "../vmisc/typedef.h"
#include "../vpatterndb/floatItemData/floatitemdef.h"

class VLayoutPieceData;
class VLayoutPiecePath;
class QGraphicsItem;
class QGraphicsPathItem;
class VTextManager;
class VPiece;
class VPieceLabelData;
class VAbstractPattern;
class VPatternLabelData;

class VLayoutPiece :public VAbstractPiece
{
    Q_DECLARE_TR_FUNCTIONS(VLayoutPiece)
public:
    VLayoutPiece();
    VLayoutPiece(const VLayoutPiece &detail);

    virtual ~VLayoutPiece() override;

    VLayoutPiece &operator=(const VLayoutPiece &detail);
#ifdef Q_COMPILER_RVALUE_REFS
    VLayoutPiece &operator=(VLayoutPiece &&detail) Q_DECL_NOTHROW;
#endif

    void Swap(VLayoutPiece &detail) Q_DECL_NOTHROW;

    static VLayoutPiece Create(const VPiece &piece, vidtype id, const VContainer *pattern);

    QVector<QPointF> GetMappedContourPoints() const;
    QVector<QPointF> GetContourPoints() const;
    void SetCountourPoints(const QVector<QPointF> &points, bool hideMainPath = false);

    QVector<QPointF> GetMappedSeamAllowancePoints() const;
    QVector<QPointF> GetSeamAllowancePoints() const;
    void SetSeamAllowancePoints(const QVector<QPointF> &points, bool seamAllowance = true,
                                bool seamAllowanceBuiltIn = false);

    QVector<QPointF> GetLayoutAllowancePoints() const;
    void SetLayoutAllowancePoints();

    QVector<VLayoutPassmark> GetPassmarks() const;
    void SetPassmarks(const QVector<VLayoutPassmark> &passmarks);

    QVector<VLayoutPlaceLabel> GetPlaceLabels() const;
    void SetPlaceLabels(const QVector<VLayoutPlaceLabel> &labels);

    QVector<QVector<QPointF>> InternalPathsForCut(bool cut) const;
    QVector<VLayoutPiecePath> GetInternalPaths() const;
    void SetInternalPaths(const QVector<VLayoutPiecePath> &internalPaths);

    QPointF GetPieceTextPosition() const;
    QStringList GetPieceText() const;
    void SetPieceText(const QString &qsName, const VPieceLabelData& data, const QFont& font, const VContainer *pattern);

    QPointF GetPatternTextPosition() const;
    QStringList GetPatternText() const;
    void SetPatternInfo(VAbstractPattern *pDoc, const VPatternLabelData& geom, const QFont& font,
                        const VContainer *pattern);

    void SetGrainline(const VGrainlineData& geom, const VContainer *pattern);
    QVector<QPointF> GetGrainline() const;
    bool  IsGrainlineEnabled() const;
    qreal GrainlineAngle() const;
    ArrowType GrainlineArrowType() const;

    QTransform GetMatrix() const;
    void    SetMatrix(const QTransform &matrix);

    qreal GetLayoutWidth() const;
    void  SetLayoutWidth(qreal value);

    quint16 GetQuantity() const;
    void    SetQuantity(quint16 value);

    vidtype GetId() const;
    void    SetId(vidtype id);

    bool IsMirror() const;
    void SetMirror(bool value);

    void Translate(qreal dx, qreal dy);
    void Rotate(const QPointF &originPoint, qreal degrees);
    void Mirror(const QLineF &edge);
    void Mirror();

    int    DetailEdgesCount() const;
    int    LayoutEdgesCount() const;

    QLineF LayoutEdge(int i) const;
    int    LayoutEdgeByPoint(const QPointF &p1) const;

    QRectF DetailBoundingRect() const;
    QRectF LayoutBoundingRect() const;
    qreal  Diagonal() const;

    static QRectF BoundingRect(QVector<QPointF> points);

    bool isNull() const;
    qint64 Square() const;

    QPainterPath ContourPath() const;
    QPainterPath LayoutAllowancePath() const;

    static QPainterPath PainterPath(const QVector<QPointF> &points);

    Q_REQUIRED_RESULT QGraphicsItem *GetItem(bool textAsPaths) const;

    bool IsLayoutAllowanceValid() const;

    qreal BiggestEdge() const;

    friend QDataStream& operator<< (QDataStream& dataStream, const VLayoutPiece& piece);
    friend QDataStream& operator>> (QDataStream& dataStream, VLayoutPiece& piece);

private:
    QSharedDataPointer<VLayoutPieceData> d;

    QVector<QPointF> DetailPath() const;

    Q_REQUIRED_RESULT QGraphicsPathItem *GetMainItem() const;
    Q_REQUIRED_RESULT QGraphicsPathItem *GetMainPathItem() const;

    void CreateLabelStrings(QGraphicsItem *parent, const QVector<QPointF> &labelShape, const VTextManager &tm,
                            bool textAsPaths) const;
    void CreateGrainlineItem(QGraphicsItem *parent) const;

    template <class T>
    QVector<T> Map(QVector<T> points) const;

    QLineF Edge(const QVector<QPointF> &path, int i) const;
    int    EdgeByPoint(const QVector<QPointF> &path, const QPointF &p1) const;
};

Q_DECLARE_TYPEINFO(VLayoutPiece, Q_MOVABLE_TYPE);

#endif // VLAYOUTDETAIL_H

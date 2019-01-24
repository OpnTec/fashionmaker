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
#include "../vgeometry/vgeometrydef.h"

class VPieceData;
class VPieceNode;
template <class T> class QVector;
template <class T>class QSharedPointer;
class VPiecePath;
class VPatternLabelData;
class VPieceLabelData;
class VGrainlineData;
class VContainer;
class QPainterPath;
class VPointF;

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

enum class PassmarkStatus: qint8
{
    Error = 0,
    Common = 1,
    Rollback = -1
};

struct VPiecePassmarkData
{
    VSAPoint previousSAPoint;
    VSAPoint passmarkSAPoint;
    VSAPoint nextSAPoint;
    qreal saWidth;
    QString nodeName;
    QString pieceName;
    PassmarkLineType passmarkLineType;
    PassmarkAngleType passmarkAngleType;
};

Q_DECLARE_METATYPE(VPiecePassmarkData)
Q_DECLARE_TYPEINFO(VPiecePassmarkData, Q_MOVABLE_TYPE);

QT_WARNING_POP

class VPiece : public VAbstractPiece
{
public:
    VPiece();
    VPiece(const VPiece &piece);

    virtual ~VPiece();

    VPiece &operator=(const VPiece &piece);
#ifdef Q_COMPILER_RVALUE_REFS
    VPiece &operator=(VPiece &&piece) Q_DECL_NOTHROW { Swap(piece); return *this; }
#endif

    inline void Swap(VPiece &piece) Q_DECL_NOTHROW
    { VAbstractPiece::Swap(piece); std::swap(d, piece.d); }

    VPiecePath GetPath() const;
    VPiecePath &GetPath();
    void       SetPath(const VPiecePath &path);

    QVector<QPointF>       MainPathPoints(const VContainer *data) const;
    QVector<VPointF>       MainPathNodePoints(const VContainer *data, bool showExcluded = false) const;
    QVector<QPointF>       SeamAllowancePoints(const VContainer *data) const;
    QVector<QPointF>       CuttingPathPoints(const VContainer *data) const;
    QVector<QLineF>        PassmarksLines(const VContainer *data) const;
    QVector<PlaceLabelImg> PlaceLabelPoints(const VContainer *data) const;

    QVector<QPainterPath> CurvesPainterPath(const VContainer *data) const;

    QPainterPath        MainPathPath(const VContainer *data) const;
    static QPainterPath MainPathPath(const QVector<QPointF> &points);

    QPainterPath SeamAllowancePath(const VContainer *data) const;
    QPainterPath SeamAllowancePath(const QVector<QPointF> &points) const;
    QPainterPath PassmarksPath(const VContainer *data) const;
    QPainterPath PlaceLabelPath(const VContainer *data) const;

    bool IsInLayout() const;
    void SetInLayout(bool inLayout);

    bool IsUnited() const;
    void SetUnited(bool united);

    QString GetFormulaSAWidth() const;
    void    SetFormulaSAWidth(const QString &formula, qreal value);

    QVector<quint32> GetInternalPaths() const;
    QVector<quint32> &GetInternalPaths();
    void             SetInternalPaths(const QVector<quint32> &iPaths);

    QVector<CustomSARecord> GetCustomSARecords() const;
    QVector<CustomSARecord> &GetCustomSARecords();
    void                    SetCustomSARecords(const QVector<CustomSARecord> &records);

    QVector<quint32> GetPins() const;
    QVector<quint32> &GetPins();
    void             SetPins(const QVector<quint32> &pins);

    QVector<quint32> GetPlaceLabels() const;
    QVector<quint32> &GetPlaceLabels();
    void             SetPlaceLabels(const QVector<quint32> &labels);

    QList<quint32> Dependencies() const;
    QVector<quint32> MissingNodes(const VPiece &det) const;
    QVector<quint32> MissingCSAPath(const VPiece &det) const;
    QVector<quint32> MissingInternalPaths(const VPiece &det) const;
    QVector<quint32> MissingPins(const VPiece &det) const;
    QVector<quint32> MissingPlaceLabels(const VPiece &det) const;

    void                   SetPatternPieceData(const VPieceLabelData &data);
    VPieceLabelData&       GetPatternPieceData();
    const VPieceLabelData& GetPatternPieceData() const;

    void                     SetPatternInfo(const VPatternLabelData &info);
    VPatternLabelData&       GetPatternInfo();
    const VPatternLabelData& GetPatternInfo() const;

    void                    SetGrainlineGeometry(const VGrainlineData &data);
    VGrainlineData&         GetGrainlineGeometry();
    const VGrainlineData&   GetGrainlineGeometry() const;

    static QVector<QLineF> SAPassmark(const VPiecePassmarkData &passmarkData, const QVector<QPointF> &seamAllowance);
protected:
    QVector<QPointF> SeamAllowancePointsWithRotation(const VContainer *data, int makeFirst) const;
private:
    QSharedDataPointer<VPieceData> d;

    QVector<VPieceNode> GetUnitedPath(const VContainer *data) const;

    QVector<CustomSARecord> GetValidRecords() const;
    QVector<CustomSARecord> FilterRecords(QVector<CustomSARecord> records) const;

    QVector<VSAPoint> GetNodeSAPoints(const QVector<VPieceNode> &path, int index, const VContainer *data) const;

    bool GetPassmarkSAPoint(const QVector<VPieceNode> &path, int index, const VContainer *data, VSAPoint &point) const;
    bool GetPassmarkPreviousSAPoints(const QVector<VPieceNode> &path, int index, const VSAPoint &passmarkSAPoint,
                                     const VContainer *data, VSAPoint &point, int passmarkIndex) const;
    bool GetPassmarkNextSAPoints(const QVector<VPieceNode> &path, int index, const VSAPoint &passmarkSAPoint,
                                 const VContainer *data, VSAPoint &point, int passmarkIndex) const;
    static PassmarkStatus GetSeamPassmarkSAPoint(VPiecePassmarkData passmarkData,
                                                   const QVector<QPointF> &seamAllowance, QPointF &point);

    bool IsPassmarkVisible(const QVector<VPieceNode> &path, int passmarkIndex) const;

    QVector<QLineF> CreatePassmark(const QVector<VPieceNode> &path, int previousIndex, int passmarkIndex, int nextIndex,
                                   const VContainer *data) const;

    static QVector<QLineF> BuiltInSAPassmark(const VPiecePassmarkData &passmarkData, const QVector<QPointF> &mainPath);
    static QVector<QLineF> PassmarkBisector(PassmarkStatus seamPassmarkType, const VPiecePassmarkData &passmarkData,
                                            const QPointF &seamPassmarkSAPoint, const QVector<QPointF>& seamAllowance);

    static int IsCSAStart(const QVector<CustomSARecord> &records, quint32 id);

    PlaceLabelImg LabelShape(const VContainer *data) const;
    PlaceLabelImg SegmentShape(const VContainer *data) const;
    PlaceLabelImg RectangleShape(const VContainer *data) const;
    PlaceLabelImg CrossShape(const VContainer *data) const;
    PlaceLabelImg TshapedShape(const VContainer *data) const;
    PlaceLabelImg DoubletreeShape(const VContainer *data) const;
    PlaceLabelImg CornerShape(const VContainer *data) const;
    PlaceLabelImg TriangleShape(const VContainer *data) const;
    PlaceLabelImg HshapedShape(const VContainer *data) const;
    PlaceLabelImg ButtonShape(const VContainer *data) const;
};

Q_DECLARE_TYPEINFO(VPiece, Q_MOVABLE_TYPE);

#endif // VPIECE_H

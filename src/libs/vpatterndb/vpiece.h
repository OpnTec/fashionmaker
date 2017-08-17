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

    void Swap(VPiece &piece) Q_DECL_NOTHROW
    { VAbstractPiece::Swap(piece); std::swap(d, piece.d); }

    VPiecePath GetPath() const;
    VPiecePath &GetPath();
    void       SetPath(const VPiecePath &path);

    QVector<QPointF> MainPathPoints(const VContainer *data) const;
    QVector<VPointF> MainPathNodePoints(const VContainer *data, bool showExcluded = false) const;
    QVector<QPointF> SeamAllowancePoints(const VContainer *data) const;
    QVector<QLineF>  PassmarksLines(const VContainer *data,
                                    const QVector<QPointF> &seamAllowance = QVector<QPointF>()) const;

    QPainterPath MainPathPath(const VContainer *data) const;
    QPainterPath SeamAllowancePath(const VContainer *data) const;
    QPainterPath SeamAllowancePath(const QVector<QPointF> &points) const;
    QPainterPath PassmarksPath(const VContainer *data,
                               const QVector<QPointF> &seamAllowance = QVector<QPointF>()) const;

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

    QVector<quint32> MissingNodes(const VPiece &det) const;
    QVector<quint32> MissingCSAPath(const VPiece &det) const;
    QVector<quint32> MissingInternalPaths(const VPiece &det) const;
    QVector<quint32> MissingPins(const VPiece &det) const;

    void                   SetPatternPieceData(const VPieceLabelData &data);
    VPieceLabelData&       GetPatternPieceData();
    const VPieceLabelData& GetPatternPieceData() const;

    void                     SetPatternInfo(const VPatternLabelData &info);
    VPatternLabelData&       GetPatternInfo();
    const VPatternLabelData& GetPatternInfo() const;

    VGrainlineData&         GetGrainlineGeometry();
    const VGrainlineData&   GetGrainlineGeometry() const;

private:
    QSharedDataPointer<VPieceData> d;

    QVector<VPieceNode> GetUnitedPath(const VContainer *data) const;

    QVector<CustomSARecord> GetValidRecords() const;
    QVector<CustomSARecord> FilterRecords(QVector<CustomSARecord> records) const;

    QVector<VSAPoint> GetNodeSAPoints(const QVector<VPieceNode> &path, int index, const VContainer *data) const;

    bool GetPassmarkSAPoint(const QVector<VPieceNode> &path, int index, const VContainer *data, VSAPoint &point) const;
    bool GetPassmarkPreviousSAPoints(const QVector<VPieceNode> &path, int index, const VSAPoint &passmarkSAPoint,
                                     const VContainer *data, VSAPoint &point) const;
    int GetPassmarkNextSAPoints(const QVector<VPieceNode> &path, int index, const VSAPoint &passmarkSAPoint,
                                const VContainer *data, VSAPoint &point) const;
    bool GetSeamPassmarkSAPoint(const VSAPoint &previousSAPoint, const VSAPoint &passmarkSAPoint,
                                const VSAPoint &nextSAPoint, const VContainer *data, QPointF &point) const;

    bool IsPassmarkVisible(const QVector<VPieceNode> &path, int passmarkIndex) const;

    QVector<QLineF> CreatePassmark(const QVector<VPieceNode> &path, int previousIndex, int passmarkIndex, int nextIndex,
                                   const VContainer *data,
                                   const QVector<QPointF> &seamAllowance = QVector<QPointF>()) const;
    QVector<QLineF> SAPassmark(const QVector<VPieceNode> &path, VSAPoint &previousSAPoint,
                               const VSAPoint &passmarkSAPoint, VSAPoint &nextSAPoint, const VContainer *data,
                               int passmarkIndex, const QVector<QPointF> &seamAllowance = QVector<QPointF>()) const;
    QVector<QLineF> BuiltInSAPassmark(const QVector<VPieceNode> &path, const VSAPoint &previousSAPoint,
                                      const VSAPoint &passmarkSAPoint, const VSAPoint &nextSAPoint,
                                      const VContainer *data, int passmarkIndex) const;

    static int IsCSAStart(const QVector<CustomSARecord> &records, quint32 id);
};

Q_DECLARE_TYPEINFO(VPiece, Q_MOVABLE_TYPE);

#endif // VPIECE_H

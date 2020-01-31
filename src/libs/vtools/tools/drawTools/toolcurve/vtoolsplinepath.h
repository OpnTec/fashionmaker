/************************************************************************
 **
 **  @file   vtoolsplinepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VTOOLSPLINEPATH_H
#define VTOOLSPLINEPATH_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QtGlobal>

#include "../vgeometry/vgeometrydef.h"
#include "../vmisc/def.h"
#include "../ifc/xml/vabstractpattern.h"
#include "vabstractspline.h"

class VSplinePath;
template <class T> class QSharedPointer;

struct VToolSplinePathInitData : VAbstractSplineInitData
{
    VToolSplinePathInitData()
        : VAbstractSplineInitData(),
          points(),
          a1(),
          a2(),
          l1(),
          l2(),
          duplicate(0)
    {}

    QVector<quint32> points;
    QVector<QString> a1;
    QVector<QString> a2;
    QVector<QString> l1;
    QVector<QString> l2;
    quint32 duplicate;
};

/**
 * @brief The VToolSplinePath class tool for creation spline path.
 */
class VToolSplinePath:public VAbstractSpline
{
    Q_OBJECT
public:
    virtual ~VToolSplinePath() =default;
    virtual void setDialog() override;
    static VToolSplinePath *Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene  *scene,
                                   VAbstractPattern *doc, VContainer *data);
    static VToolSplinePath *Create(VToolSplinePathInitData &initData, VSplinePath *path);
    static VToolSplinePath *Create(VToolSplinePathInitData &initData);
    static const QString ToolType;
    static const QString OldToolType;
    static void  UpdatePathPoints(VAbstractPattern *doc, QDomElement &element, const VSplinePath &path);
    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::SplinePath)};

    VSplinePath getSplinePath()const;
    void        setSplinePath(const VSplinePath &splPath);

    virtual void ShowVisualization(bool show) override;
signals:
    /**
     * @brief RefreshLine refresh control line.
     * @param indexSpline position spline in spline list.
     * @param pos position point in spline.
     * @param controlPoint new position control point.
     * @param splinePoint new position spline point.
     */
    void          RefreshLine(const qint32 &indexSpline, SplinePointPosition pos, const QPointF &controlPoint,
                              const QPointF &splinePoint);
public slots:

    void          ControlPointChangePosition(const qint32 &indexSpline, const SplinePointPosition &position,
                                             const QPointF &pos);
    virtual void  EnableToolMove(bool move) override;
protected slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
protected:
    virtual void RemoveReferens() override;
    virtual void SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                            QList<quint32> &newDependencies) override;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) override;
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) override;
    virtual void SetVisualization() override;
    virtual void RefreshCtrlPoints() override;
private slots:
    void CurveReleased();
private:
    Q_DISABLE_COPY(VToolSplinePath)
    QPointF oldPosition;
    int     splIndex;

    bool moved;
    QSharedPointer<VSplinePath> oldMoveSplinePath;
    QSharedPointer<VSplinePath> newMoveSplinePath;

    VToolSplinePath(const VToolSplinePathInitData &initData, QGraphicsItem *parent = nullptr);

    bool          IsMovable(int index) const;
    static void   AddPathPoint(VAbstractPattern *doc, QDomElement &domElement, const VSplinePoint &splPoint);
    void          UpdateControlPoints(const VSpline &spl, QSharedPointer<VSplinePath> &splPath,
                                      qint32 indexSpline) const;
    void          SetSplinePathAttributes(QDomElement &domElement, const VSplinePath &path);

    void UndoCommandMove(const VSplinePath &oldPath, const VSplinePath &newPath);
};

#endif // VTOOLSPLINEPATH_H

/************************************************************************
 **
 **  @file   vtoolspline.h
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

#ifndef VTOOLSPLINE_H
#define VTOOLSPLINE_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>

#include "../vgeometry/vgeometrydef.h"
#include "../vmisc/def.h"
#include "../ifc/xml/vabstractpattern.h"
#include "vabstractspline.h"
#include "../vgeometry/vspline.h"

template <class T> class QSharedPointer;

struct VToolSplineInitData : VAbstractSplineInitData
{
    VToolSplineInitData()
        : VAbstractSplineInitData(),
          point1(NULL_ID),
          point4(NULL_ID),
          a1(),
          a2(),
          l1(),
          l2(),
          duplicate(0)
    {}

    quint32 point1;
    quint32 point4;
    QString a1;
    QString a2;
    QString l1;
    QString l2;
    quint32 duplicate;
};

/**
 * @brief The VToolSpline class tool for creation spline. I mean bezier curve.
 */
class VToolSpline:public VAbstractSpline
{
    Q_OBJECT
public:
    virtual ~VToolSpline() =default;
    virtual void setDialog() override;
    static VToolSpline *Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                               VContainer *data);
    static VToolSpline *Create(VToolSplineInitData &initData, VSpline *spline);
    static VToolSpline *Create(VToolSplineInitData &initData);
    static const QString ToolType;
    static const QString OldToolType;
    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Spline)};

    VSpline getSpline()const;
    void    setSpline(const VSpline &spl);

    virtual void ShowVisualization(bool show) override;
public slots:
    void         ControlPointChangePosition (const qint32 &indexSpline, const SplinePointPosition &position,
                                             const QPointF &pos);
    virtual void EnableToolMove(bool move) override;
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
    Q_DISABLE_COPY(VToolSpline)
    QPointF oldPosition;

    bool moved;
    QSharedPointer<VSpline> oldMoveSpline;
    QSharedPointer<VSpline> newMoveSpline;

    VToolSpline (const VToolSplineInitData &initData, QGraphicsItem *parent = nullptr );

    bool IsMovable() const;
    void SetSplineAttributes(QDomElement &domElement, const VSpline &spl);

    void UndoCommandMove(const VSpline &oldSpl, const VSpline &newSpl);
};

#endif // VTOOLSPLINE_H

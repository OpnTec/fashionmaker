/************************************************************************
 **
 **  @file   vtoolspline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

template <class T> class QSharedPointer;

/**
 * @brief The VToolSpline class tool for creation spline. I mean bezier curve.
 */
class VToolSpline:public VAbstractSpline
{
    Q_OBJECT
public:
    virtual ~VToolSpline() =default;
    virtual void setDialog() Q_DECL_OVERRIDE;
    static VToolSpline *Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                               VContainer *data);
    static VToolSpline *Create(const quint32 _id, VSpline *spline,
                               VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                               const Document &parse, const Source &typeCreation);
    static VToolSpline *Create(const quint32 _id, quint32 point1, quint32 point4, QString &a1, QString &a2, QString &l1,
                               QString &l2, quint32 duplicate, const QString &color, const QString &penStyle,
                               VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                               const Document &parse, const Source &typeCreation);
    static const QString ToolType;
    static const QString OldToolType;
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Spline)};

    VSpline getSpline()const;
    void    setSpline(const VSpline &spl);

    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;
public slots:
    void         ControlPointChangePosition (const qint32 &indexSpline, const SplinePointPosition &position,
                                             const QPointF &pos);
    virtual void EnableToolMove(bool move) Q_DECL_OVERRIDE;
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void RemoveReferens() Q_DECL_OVERRIDE;
    virtual void SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void SetVisualization() Q_DECL_OVERRIDE;
    virtual void RefreshCtrlPoints() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolSpline)
    QPointF oldPosition;

    VToolSpline (VAbstractPattern *doc, VContainer *data, quint32 id, const Source &typeCreation,
                 QGraphicsItem * parent = nullptr );

    bool IsMovable() const;
    void SetSplineAttributes(QDomElement &domElement, const VSpline &spl);
};

#endif // VTOOLSPLINE_H

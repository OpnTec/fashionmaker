/************************************************************************
 **
 **  @file   vabstractspline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   4 3, 2014
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

#ifndef VABSTRACTSPLINE_H
#define VABSTRACTSPLINE_H

#include "../vdrawtool.h"
#include "../../../visualization/line/visline.h"

#include <QGraphicsPathItem>

class VControlPointSpline;
class VSpline;

class VAbstractSpline:public VDrawTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VAbstractSpline(VAbstractPattern *doc, VContainer *data, quint32 id, QGraphicsItem * parent = nullptr);
    virtual ~VAbstractSpline() Q_DECL_OVERRIDE;

    virtual int      type() const  Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::AbstractSpline)};
    virtual QString  getTagName() const Q_DECL_OVERRIDE;
    void             ShowHandles(bool show);

    void setEnabled(bool enabled);

    QString GetLineColor() const;
    void    SetLineColor(const QString &value);

    QString name() const;

    virtual void GroupVisibility(quint32 object, bool visible) Q_DECL_OVERRIDE;
public slots:
    virtual void     FullUpdateFromFile () Q_DECL_OVERRIDE;
    virtual void     Disable(bool disable, const QString &namePP) Q_DECL_OVERRIDE;
    virtual void     DetailsMode(bool mode) Q_DECL_OVERRIDE;
    virtual void     AllowHover(bool enabled) Q_DECL_OVERRIDE;
    virtual void     AllowSelecting(bool enabled) Q_DECL_OVERRIDE;
    virtual void     SetFactor(qreal factor) Q_DECL_OVERRIDE;
signals:
    /**
     * @brief setEnabledPoint disable control points.
     * @param enable enable or diasable points.
     */
    void             setEnabledPoint(bool enable);
protected:
    /**
     * @brief controlPoints list pointers of control points.
     */
    QVector<VControlPointSpline *>   controlPoints;
    SceneObject      sceneType;
    bool             isHovered;
    bool             detailsMode;
    /**
     * @brief RefreshGeometry  refresh item on scene.
     */
    virtual void     RefreshGeometry ()=0;
    virtual void     ShowTool(quint32 id, bool enable) Q_DECL_OVERRIDE;
    virtual void     hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void     hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value ) Q_DECL_OVERRIDE;
    virtual void     keyReleaseEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    virtual void     mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void     mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    QPainterPath     ToolPath(PathDirection direction = PathDirection::Hide) const;
    virtual void     ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void     SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;

    VSpline CorrectedSpline(const VSpline &spline, const SplinePointPosition &position, const QPointF &pos) const;

    template <typename T>
    void ShowToolVisualization(bool show);

    template <typename T>
    static void InitSplineToolConnections(VMainGraphicsScene *scene, T *tool);

    template <typename T>
    static void InitSplinePathToolConnections(VMainGraphicsScene *scene, T *tool);

    template <typename T>
    static void InitArcToolConnections(VMainGraphicsScene *scene, T *tool);

private:
    Q_DISABLE_COPY(VAbstractSpline)
};

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void VAbstractSpline::ShowToolVisualization(bool show)
{
    if (show)
    {
        if (vis == nullptr)
        {
            AddVisualization<T>();
            SetVisualization();
        }
        else
        {
            if (T *visual = qobject_cast<T *>(vis))
            {
                visual->show();
            }
        }
    }
    else
    {
        delete vis;
        vis = nullptr;
    }

    if (detailsMode)
    {
        ShowHandles(detailsMode);
    }
    else
    {
        ShowHandles(show);
    }

    if (scene())
    { // Showing/hiding control points require recalculation scene size.
        VMainGraphicsView::NewSceneRect(scene(), qApp->getSceneView());
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractSpline::InitSplineToolConnections(VMainGraphicsScene *scene, T *tool)
{
    SCASSERT(scene != nullptr);
    SCASSERT(tool != nullptr);

    InitDrawToolConnections(scene, tool);
    QObject::connect(scene, &VMainGraphicsScene::EnableSplineItemHover, tool, &T::AllowHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableSplineItemSelection, tool, &T::AllowSelecting);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractSpline::InitSplinePathToolConnections(VMainGraphicsScene *scene, T *tool)
{
    SCASSERT(scene != nullptr);
    SCASSERT(tool != nullptr);

    InitDrawToolConnections(scene, tool);
    QObject::connect(scene, &VMainGraphicsScene::EnableSplinePathItemHover, tool, &T::AllowHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableSplinePathItemSelection, tool, &T::AllowSelecting);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractSpline::InitArcToolConnections(VMainGraphicsScene *scene, T *tool)
{
    SCASSERT(scene != nullptr);
    SCASSERT(tool != nullptr);

    InitDrawToolConnections(scene, tool);
    QObject::connect(scene, &VMainGraphicsScene::EnableArcItemHover, tool, &T::AllowHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableArcItemSelection, tool, &T::AllowSelecting);
}

#endif // VABSTRACTSPLINE_H

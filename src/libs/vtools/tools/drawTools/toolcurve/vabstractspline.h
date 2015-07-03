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
#include "../vwidgets/vcontrolpointspline.h"

#include <QGraphicsPathItem>

class VAbstractSpline:public VDrawTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VAbstractSpline(VAbstractPattern *doc, VContainer *data, quint32 id, QGraphicsItem * parent = nullptr);
    virtual ~VAbstractSpline() Q_DECL_OVERRIDE;
    // cppcheck-suppress duplInheritedMember
    static const QString TagName;
    virtual void     paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
                           QWidget * widget = 0) Q_DECL_OVERRIDE;
    virtual int      type() const  Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::AbstractSpline)};
    virtual QString  getTagName() const Q_DECL_OVERRIDE;
    void             ShowHandles(bool show);

    void setEnabled(bool enabled);

public slots:
    virtual void     FullUpdateFromFile () Q_DECL_OVERRIDE;
    virtual void     Disable(bool disable, const QString &namePP) Q_DECL_OVERRIDE;
    void             DetailsMode(bool mode);
signals:
    /**
     * @brief RefreshLine refresh control line.
     * @param indexSpline position spline in spline list.
     * @param pos position point in spline.
     * @param controlPoint new position control point.
     * @param splinePoint new position spline point.
     */
    void             RefreshLine(const qint32 &indexSpline, SplinePointPosition pos,
                                 const QPointF &controlPoint, const QPointF &splinePoint);
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
    virtual void     SetFactor(qreal factor) Q_DECL_OVERRIDE;
    virtual void     hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void     hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value ) Q_DECL_OVERRIDE;
    virtual void     keyReleaseEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    virtual void     mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    QPainterPath     ToolPath(PathDirection direction = PathDirection::Hide) const;
    virtual void     ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void     SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;

    template <typename T>
    void ShowToolVisualization(bool show)
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
    }

private:
    Q_DISABLE_COPY(VAbstractSpline)
};

#endif // VABSTRACTSPLINE_H

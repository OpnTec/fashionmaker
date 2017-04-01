/************************************************************************
 **
 **  @file   vabstractpoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VABSTRACTPOINT_H
#define VABSTRACTPOINT_H

#include <qcompilerdetection.h>
#include <QColor>
#include <QMetaObject>
#include <QObject>
#include <QPen>
#include <QPointF>
#include <QString>
#include <Qt>
#include <QtGlobal>

#include "../vdrawtool.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vmisc/def.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../visualization/visualization.h"
#include "../../vabstracttool.h"

class VAbstractPoint: public VDrawTool
{
    Q_OBJECT
public:
    VAbstractPoint(VAbstractPattern *doc, VContainer *data, quint32 id);
    virtual ~VAbstractPoint() Q_DECL_EQ_DEFAULT;

    virtual QString      getTagName() const Q_DECL_OVERRIDE;

    template <typename T>
    void ShowToolVisualization(bool show);

public slots:
    virtual void ShowTool(quint32 id, bool enable) Q_DECL_OVERRIDE;
    void         DeleteFromLabel();
    virtual void DoChangePosition(quint32 id, qreal mx, qreal my) =0;

protected:
    void SetPointName(quint32 id, const QString &name);

    template <typename T>
    void ChangePosition(T *item, quint32 id, const QPointF &pos);


    virtual void UpdateNamePosition(quint32 id)=0;
    virtual void RefreshLine(quint32 id)=0;

    template <typename T>
    void SetToolEnabled(T *item, const QColor &color, bool enabled);

    template <typename T>
    static void InitToolConnections(VMainGraphicsScene *scene, T *tool);

private:
    Q_DISABLE_COPY(VAbstractPoint)
};

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractPoint::ShowToolVisualization(bool show)
{
    if (show)
    {
        if (vis.isNull())
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
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractPoint::SetToolEnabled(T *item, const QColor &color, bool enabled)
{
    item->setEnabled(enabled);
    if (enabled)
    {
        item->setPen(QPen(color,
                          qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    }
    else
    {
        item->setPen(QPen(Qt::gray, qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))/factor));
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractPoint::ChangePosition(T *item, quint32 id, const QPointF &pos)
{
    const QPointF p = pos - item->pos();
    DoChangePosition(id, p.x(), p.y());
    UpdateNamePosition(id);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractPoint::InitToolConnections(VMainGraphicsScene *scene, T *tool)
{
    SCASSERT(scene != nullptr)
    SCASSERT(tool != nullptr)

    InitDrawToolConnections(scene, tool);
    QObject::connect(scene, &VMainGraphicsScene::EnablePointItemHover, tool, &T::AllowHover);
    QObject::connect(scene, &VMainGraphicsScene::EnablePointItemSelection, tool, &T::AllowSelecting);
    QObject::connect(scene, &VMainGraphicsScene::EnableLabelItemHover, tool, &T::AllowLabelHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableLabelItemSelection, tool, &T::AllowLabelSelecting);
}

#endif // VABSTRACTPOINT_H

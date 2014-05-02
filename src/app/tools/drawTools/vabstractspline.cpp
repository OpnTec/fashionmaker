/************************************************************************
 **
 **  @file   vabstractspline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   4 3, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "vabstractspline.h"
#include <QKeyEvent>

const QString VAbstractSpline::TagName = QStringLiteral("spline");

VAbstractSpline::VAbstractSpline(VPattern *doc, VContainer *data, quint32 id, QGraphicsItem *parent)
    :VDrawTool(doc, data, id), QGraphicsPathItem(parent), controlPoints(QVector<VControlPointSpline *>())
{
    ignoreFullUpdate = true;
}

void VAbstractSpline::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VAbstractSpline::ChangedActivDraw(const QString &newName)
{
    bool selectable = false;
    if (nameActivDraw == newName)
    {
        selectable = true;
        currentColor = Qt::black;
    }
    else
    {
        selectable = false;
        currentColor = Qt::gray;
    }
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, selectable);
    this->setAcceptHoverEvents (selectable);
    emit setEnabledPoint(selectable);
    VDrawTool::ChangedActivDraw(newName);
}

void VAbstractSpline::ShowTool(quint32 id, Qt::GlobalColor color, bool enable)
{
    ShowItem(this, id, color, enable);
}

void VAbstractSpline::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

// cppcheck-suppress unusedFunction
void VAbstractSpline::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthMainLine())/factor));
}

// cppcheck-suppress unusedFunction
void VAbstractSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor));
}

QVariant VAbstractSpline::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
    {
        if (value == true)
        {
            // do stuff if selected
            this->setFocus();
        }
        else
        {
            // do stuff if not selected
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void VAbstractSpline::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            DeleteTool(this);
            break;
        default:
            break;
    }
    QGraphicsItem::keyReleaseEvent ( event );
}

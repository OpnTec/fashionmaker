/************************************************************************
 **
 **  @file   vwidgetpopup.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 2, 2015
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

#include "vwidgetpopup.h"

#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QTimer>

//---------------------------------------------------------------------------------------------------------------------
VWidgetPopup::VWidgetPopup(QWidget *parent)
    :QFrame(parent, Qt::Popup), mWidget(nullptr), mOwn(true), mOldParent(nullptr), lifeTime(-1)
{
    setAttribute(Qt::WA_WindowPropagation);

    if (parentWidget() == nullptr)
    {
        setAttribute(Qt::WA_DeleteOnClose);
    }

    setLayout(new QVBoxLayout());
    layout()->setContentsMargins(0, 0, 0, 0);
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetPopup::SetWidget(QWidget *widget, bool own)
{
    if (mWidget)
    {
        layout()->removeWidget(mWidget);

        if (mOwn)
        {
            mWidget->setParent(0);
            delete mWidget;
        }
        else
        {
            mWidget->setParent(mOldParent);
        }
    }

    mWidget = widget;
    mOwn = own;
    mOldParent = 0;

    if (mWidget)
    {
        mOldParent = mWidget->parentWidget();
        mWidget->setParent(this);
        layout()->addWidget(mWidget);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetPopup::Show(QPoint coord)
{
    // important to do this before following adjustments!
    QFrame::show();

    const QRect screen(QDesktopWidget().availableGeometry());
    coord.setX(coord.x() - width()/2);

    if (coord.x() < screen.x())
    {
        coord.setX(screen.x());
    }

    if (coord.y() < screen.y())
    {
        coord.setY(screen.y());
    }

    if (coord.x() > (screen.right()-width()))
    {
        coord.setX(screen.right()-width());
    }

    if (coord.y() > (screen.bottom()-height()))
    {
        coord.setY(screen.bottom()-height());
    }
    move(coord);

    if (lifeTime > 0)
    {
        QTimer::singleShot(lifeTime, this, SLOT(close()));
    }
}

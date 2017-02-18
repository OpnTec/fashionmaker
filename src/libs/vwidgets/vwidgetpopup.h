/************************************************************************
 **
 **  @file   vwidgetpopup.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 2, 2015
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

#ifndef VWIDGETPOPUP_H
#define VWIDGETPOPUP_H

#include <QFrame>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

/**
    \brief Class showing a widget as popup window.

    setWidget() function allows you to specify the widget to be popped up.
    After widget is set, you normally should call show() slot in order to pop the
    widget up at the specified global position.

    VWidgetPopup takes care about positioning of your widget on the screen so it will
    be always visible even if popped beside.
*/
class VWidgetPopup : public QFrame
{
    Q_OBJECT

public:
    /** Constructor.

        If \a parent not specified (default), then popup widget gets
        attribute Qt::WA_DeleteOnClose and will be deleted after close.
      */
    explicit VWidgetPopup(QWidget *parent = 0);

    /** Sets widget to be popped up to \a widget.
        If \a own is true then the widget will be reparented to the popup widget.
      */
    void SetWidget(QWidget *widget, bool own = true);

    /** Returns widget to be popped up. */
    inline QWidget* Widget() const;

    /** Returns true if widget is owned by this popup widget, false otherwise. */
    inline bool isOwned() const;

    int GetLifeTime() const;
    void SetLifeTime(int value);

    static void PopupMessage(QWidget *w, const QString &msg);

public slots:
    /** Pops up the widget at global coordinates \a coord. */
    void Show(QPoint coord);

protected:
    Q_DISABLE_COPY(VWidgetPopup)
    QWidget *mWidget;
    bool mOwn;
    QWidget *mOldParent;
    int lifeTime;
};

//---------------------------------------------------------------------------------------------------------------------
inline QWidget *VWidgetPopup::Widget() const
{
    return mWidget;
}

//---------------------------------------------------------------------------------------------------------------------
inline bool VWidgetPopup::isOwned() const
{
    return mOwn;
}

//---------------------------------------------------------------------------------------------------------------------
inline int VWidgetPopup::GetLifeTime() const
{
    return lifeTime;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VWidgetPopup::SetLifeTime(int value)
{
    lifeTime = value;
}

#endif // VWIDGETPOPUP_H

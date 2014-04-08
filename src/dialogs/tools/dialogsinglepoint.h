/************************************************************************
 **
 **  @file   dialogsinglepoint.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
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

#ifndef DIALOGSINGLEPOINT_H
#define DIALOGSINGLEPOINT_H

#include "dialogtool.h"

namespace Ui
{
    class DialogSinglePoint;
}

/**
 * @brief The DialogSinglePoint class dialog for ToolSinglePoint. Help create point and edit option.
 */
class DialogSinglePoint : public DialogTool
{
    Q_OBJECT
public:
    /**
     * @brief DialogSinglePoint create dialog
     * @param data container with data
     * @param parent parent widget
     */
    DialogSinglePoint(const VContainer *data, QWidget *parent = nullptr);
    ~DialogSinglePoint();
    /**
     * @brief setData set name and point
     * @param name name of point
     * @param point data for point
     */
    void           setData(const QString &name, const QPointF &point);
    /**
     * @brief getName return name
     * @return name
     */
    QString        getName()const;
    /**
     * @brief getPoint return point
     * @return point
     */
    QPointF        getPoint()const;
public slots:
    /**
     * @brief mousePress get mouse position
     * @param scenePos position of cursor
     */
    void           mousePress(const QPointF &scenePos);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void   DialogAccepted();
private:
    Q_DISABLE_COPY(DialogSinglePoint)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogSinglePoint *ui;
    /**
     * @brief name name of point
     */
    QString        name;
    /**
     * @brief point data of point
     */
    QPointF        point;
};

inline QString DialogSinglePoint::getName() const
{
    return name;
}

inline QPointF DialogSinglePoint::getPoint() const
{
    return point;
}

#endif // DIALOGSINGLEPOINT_H

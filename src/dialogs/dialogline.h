/************************************************************************
 **
 **  @file   dialogline.h
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

#ifndef DIALOGLINE_H
#define DIALOGLINE_H

#include "dialogtool.h"

namespace Ui
{
    class DialogLine;
}

/**
 * @brief The DialogLine class dialog for ToolLine. Help create line and edit option.
 */
class DialogLine : public DialogTool
{
    Q_OBJECT
public:
                     /**
                      * @brief DialogLine create dialog
                      * @param data container with data
                      * @param parent parent widget
                      */
                     DialogLine(const VContainer *data, QWidget *parent = 0);
                     ~DialogLine();
    /**
     * @brief getFirstPoint return id first point
     * @return id
     */
    inline qint64    getFirstPoint() const {return firstPoint;}
    /**
     * @brief setFirstPoint set id first point
     * @param value id
     */
    void             setFirstPoint(const qint64 &value);
    /**
     * @brief getSecondPoint return id second point
     * @return id
     */
    inline qint64    getSecondPoint() const {return secondPoint;}
    /**
     * @brief setSecondPoint set id second point
     * @param value id
     */
    void             setSecondPoint(const qint64 &value);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    void             ChoosedObject(qint64 id, const Scene::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void     DialogAccepted();
private:
    Q_DISABLE_COPY(DialogLine)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogLine   *ui;
    /**
     * @brief number number of handled objects
     */
    qint32           number;
    /**
     * @brief firstPoint id first point
     */
    qint64           firstPoint;
    /**
     * @brief secondPoint id second point
     */
    qint64           secondPoint;
};

#endif // DIALOGLINE_H

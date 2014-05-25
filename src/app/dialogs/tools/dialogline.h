/************************************************************************
 **
 **  @file   dialogline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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
    DialogLine(const VContainer *data, QWidget *parent = nullptr);
    ~DialogLine();
    /**
     * @brief getFirstPoint return id first point
     * @return id
     */
    quint32        getFirstPoint() const;
    /**
     * @brief setFirstPoint set id first point
     * @param value id
     */
    void           setFirstPoint(const quint32 &value);
    /**
     * @brief getSecondPoint return id second point
     * @return id
     */
    quint32        getSecondPoint() const;
    /**
     * @brief setSecondPoint set id second point
     * @param value id
     */
    void           setSecondPoint(const quint32 &value);
    /**
     * @brief getTypeLine return type of line
     * @return type
     */
    QString        getTypeLine() const;
    /**
     * @brief setTypeLine set type of line
     * @param value type
     */
    void           setTypeLine(const QString &value);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void   ChoosedObject(quint32 id, const Valentina::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void   DialogAccepted();
    /** TODO ISSUE 79 : create real function
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void      DialogApply(){}
private:
    Q_DISABLE_COPY(DialogLine)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogLine *ui;
    /**
     * @brief number number of handled objects
     */
    qint32         number;
    /**
     * @brief firstPoint id first point
     */
    quint32        firstPoint;
    /**
     * @brief secondPoint id second point
     */
    quint32        secondPoint;
    /**
     * @brief typeLine type of line
     */
    QString        typeLine;
};

inline quint32 DialogLine::getFirstPoint() const
{
    return firstPoint;
}

inline quint32 DialogLine::getSecondPoint() const
{
    return secondPoint;
}

inline QString DialogLine::getTypeLine() const
{
    return typeLine;
}

#endif // DIALOGLINE_H

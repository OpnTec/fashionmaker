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
    DialogLine(const VContainer *data, QWidget *parent = nullptr);
    ~DialogLine();

    quint32        getFirstPoint() const;
    void           setFirstPoint(const quint32 &value);

    quint32        getSecondPoint() const;
    void           setSecondPoint(const quint32 &value);

    QString        getTypeLine() const;
    void           setTypeLine(const QString &value);
public slots:
    virtual void   ChoosedObject(quint32 id, const Valentina::Scenes &type);
    virtual void   DialogAccepted();
private:
    Q_DISABLE_COPY(DialogLine)

    /** @brief ui keeps information about user interface */
    Ui::DialogLine *ui;

    /** @brief number number of handled objects */
    qint32         number;

    /** @brief firstPoint id first point */
    quint32        firstPoint;

    /** @brief secondPoint id second point */
    quint32        secondPoint;

    /** @brief typeLine type of line */
    QString        typeLine;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getFirstPoint return id first point
 * @return id
 */
inline quint32 DialogLine::getFirstPoint() const
{
    return firstPoint;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getSecondPoint return id second point
 * @return id
 */
inline quint32 DialogLine::getSecondPoint() const
{
    return secondPoint;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTypeLine return type of line
 * @return type
 */
inline QString DialogLine::getTypeLine() const
{
    return typeLine;
}

#endif // DIALOGLINE_H

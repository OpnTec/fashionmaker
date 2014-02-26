/************************************************************************
 **
 **  @file   dialogalongline.h
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

#ifndef DIALOGALONGLINE_H
#define DIALOGALONGLINE_H

#include "dialogtool.h"

namespace Ui
{
    class DialogAlongLine;
}

/**
 * @brief The DialogAlongLine class dialog for ToolAlongLine. Help create point and edit option.
 */
class DialogAlongLine : public DialogTool
{
    Q_OBJECT
public:
    /**
     * @brief DialogAlongLine create dialog
     * @param data container with data
     * @param parent parent widget
     */
    DialogAlongLine(const VContainer *data, QWidget *parent = 0);
    ~DialogAlongLine();
    /**
     * @brief getPointName return name of point
     * @return name
     */
    QString             getPointName() const;
    /**
     * @brief setPointName set name of point
     * @param value name
     */
    void                setPointName(const QString &value);
    /**
     * @brief getTypeLine return type of line
     * @return type
     */
    QString             getTypeLine() const;
    /**
     * @brief setTypeLine set type of line
     * @param value type
     */
    void                setTypeLine(const QString &value);
    /**
     * @brief getFormula return string of formula
     * @return formula
     */
    QString             getFormula() const;
    /**
     * @brief setFormula set string of formula
     * @param value formula
     */
    void                setFormula(const QString &value);
    /**
     * @brief getFirstPointId return id first point of line
     * @return id
     */
    quint32             getFirstPointId() const;
    /**
     * @brief setFirstPointId set id first point of line
     * @param value id
     * @param id id of current point
     */
    void                setFirstPointId(const quint32 &value, const quint32 &id);
    /**
     * @brief getSecondPointId return id second point of line
     * @return id
     */
    quint32             getSecondPointId() const;
    /**
     * @brief setSecondPointId set id second point of line
     * @param value id
     * @param id id of current point
     */
    void                setSecondPointId(const quint32 &value, const quint32 &id);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void        ChoosedObject(quint32 id, const Scene::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void        DialogAccepted();
private:
    Q_DISABLE_COPY(DialogAlongLine)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogAlongLine *ui;
    /**
     * @brief number number of handled objects
     */
    qint32              number;
    /**
     * @brief pointName name of point
     */
    QString             pointName;
    /**
     * @brief typeLine type of line
     */
    QString             typeLine;
    /**
     * @brief formula formula
     */
    QString             formula;
    /**
     * @brief firstPointId id first point of line
     */
    quint32             firstPointId;
    /**
     * @brief secondPointId id second point of line
     */
    quint32             secondPointId;
};

inline QString DialogAlongLine::getPointName() const
{
    return pointName;
}

inline QString DialogAlongLine::getTypeLine() const
{
    return typeLine;
}

inline QString DialogAlongLine::getFormula() const
{
    return formula;
}

inline quint32 DialogAlongLine::getFirstPointId() const
{
    return firstPointId;
}

inline quint32 DialogAlongLine::getSecondPointId() const
{
    return secondPointId;
}

#endif // DIALOGALONGLINE_H

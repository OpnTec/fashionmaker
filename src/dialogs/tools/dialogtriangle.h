/************************************************************************
 **
 **  @file   dialogtriangle.h
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

#ifndef DIALOGTRIANGLE_H
#define DIALOGTRIANGLE_H

#include "dialogtool.h"

namespace Ui
{
    class DialogTriangle;
}

/**
 * @brief The DialogTriangle class dialog for ToolTriangle. Help create point and edit option.
 */
class DialogTriangle : public DialogTool
{
    Q_OBJECT
public:
    /**
     * @brief DialogTriangle create dialog
     * @param data container with data
     * @param parent parent widget
     */
    DialogTriangle(const VContainer *data, QWidget *parent = 0);
    ~DialogTriangle();
    /**
     * @brief getAxisP1Id return id first point of axis
     * @return id
     */
    quint32        getAxisP1Id() const;
    /**
     * @brief setAxisP1Id set id first point of axis
     * @param value id
     * @param id don't show this point in list
     */
    void           setAxisP1Id(const quint32 &value, const quint32 &id);
    /**
     * @brief getAxisP2Id return id second point of axis
     * @return id
     */
    quint32        getAxisP2Id() const;
    /**
     * @brief setAxisP2Id set id second point of axis
     * @param value id
     * @param id don't show this point in list
     */
    void           setAxisP2Id(const quint32 &value, const quint32 &id);
    /**
     * @brief getFirstPointId return id of first point
     * @return id
     */
    quint32        getFirstPointId() const;
    /**
     * @brief setFirstPointId set id of first point
     * @param value id
     * @param id don't show this point in list
     */
    void           setFirstPointId(const quint32 &value, const quint32 &id);
    /**
     * @brief getSecondPointId return id of second point
     * @return id
     */
    quint32        getSecondPointId() const;
    /**
     * @brief setSecondPointId set id of second point
     * @param value id
     * @param id don't show this point in list
     */
    void           setSecondPointId(const quint32 &value, const quint32 &id);
    /**
     * @brief getPointName return name of point
     * @return name
     */
    QString        getPointName() const;
    /**
     * @brief setPointName set name of point
     * @param value name
     */
    void           setPointName(const QString &value);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void   ChoosedObject(quint32 id, const Scene::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void   DialogAccepted();
private:
    Q_DISABLE_COPY(DialogTriangle)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogTriangle *ui;
    /**
     * @brief number number of handled objects
     */
    qint32         number;
    /**
     * @brief pointName name of point
     */
    QString        pointName;
    /**
     * @brief axisP1Id id first point of axis
     */
    quint32         axisP1Id;
    /**
     * @brief axisP2Id id second point of axis
     */
    quint32         axisP2Id;
    /**
     * @brief firstPointId id first point of line
     */
    quint32         firstPointId;
    /**
     * @brief secondPointId id second point of line
     */
    quint32         secondPointId;
};

inline quint32 DialogTriangle::getAxisP1Id() const
{
    return axisP1Id;
}

inline quint32 DialogTriangle::getAxisP2Id() const
{
    return axisP2Id;
}

inline quint32 DialogTriangle::getFirstPointId() const
{
    return firstPointId;
}

inline quint32 DialogTriangle::getSecondPointId() const
{
    return secondPointId;
}

inline QString DialogTriangle::getPointName() const
{
    return pointName;
}

#endif // DIALOGTRIANGLE_H

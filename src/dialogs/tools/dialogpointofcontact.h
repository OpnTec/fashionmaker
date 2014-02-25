/************************************************************************
 **
 **  @file   dialogpointofcontact.h
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

#ifndef DIALOGPOINTOFCONTACT_H
#define DIALOGPOINTOFCONTACT_H

#include "ui_dialogpointofcontact.h"
#include "dialogtool.h"

/**
 * @brief The DialogPointOfContact class dialog for ToolPointOfContact. Help create point and edit option.
 */
class DialogPointOfContact : public DialogTool
{
    Q_OBJECT
public:
                 /**
                  * @brief DialogPointOfContact create dialog
                  * @param data container with data
                  * @param parent parent widget
                  */
                 DialogPointOfContact(const VContainer *data, QWidget *parent = 0);
    /**
     * @brief getPointName return name of point
     * @return name
     */
    inline QString getPointName() const {return pointName;}
    /**
     * @brief setPointName set name of point
     * @param value name
     */
    void           setPointName(const QString &value);
    /**
     * @brief getRadius return formula radius of arc
     * @return formula
     */
    inline QString getRadius() const {return radius;}
    /**
     * @brief setRadius set formula radius of arc
     * @param value formula
     */
    void           setRadius(const QString &value);
    /**
     * @brief GetCenter return id of center point
     * @return id
     */
    inline quint32  getCenter() const {return center;}
    /**
     * @brief SetCenter set id of center point
     * @param value id
     * @param id don't show this id in list.
     */
    void           setCenter(const quint32 &value, const quint32 &id);
    /**
     * @brief getFirstPoint return id first point
     * @return id
     */
    inline quint32  getFirstPoint() const {return firstPoint;}
    /**
     * @brief setFirstPoint set id first point
     * @param value id
     * @param id don't show this id in list.
     */
    void           setFirstPoint(const quint32 &value, const quint32 &id);
    /**
     * @brief getSecondPoint return id second point
     * @return id
     */
    inline quint32  getSecondPoint() const {return secondPoint;}
    /**
     * @brief setSecondPoint set id second point
     * @param value id
     * @param id don't show this id in list.
     */
    void           setSecondPoint(const quint32 &value, const quint32 &id);
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
    Q_DISABLE_COPY(DialogPointOfContact)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogPointOfContact ui;
    /**
     * @brief number number of handled objects
     */
    qint32         number;
    /**
     * @brief pointName name of point
     */
    QString        pointName;
    /**
     * @brief radius radius of arc
     */
    QString        radius;
    /**
     * @brief center id center point of arc
     */
    quint32         center;
    /**
     * @brief firstPoint id first point of line
     */
    quint32         firstPoint;
    /**
     * @brief secondPoint id second point of line
     */
    quint32         secondPoint;
};

#endif // DIALOGPOINTOFCONTACT_H

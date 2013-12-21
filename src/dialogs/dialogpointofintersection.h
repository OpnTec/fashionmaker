/************************************************************************
 **
 **  @file   dialogpointofintersection.h
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

#ifndef DIALOGPOINTOFINTERSECTION_H
#define DIALOGPOINTOFINTERSECTION_H

#include "dialogtool.h"

namespace Ui
{
    class DialogPointOfIntersection;
}

/**
 * @brief The DialogPointOfIntersection class dialog for ToolPointOfIntersection. Help create point and edit option.
 */
class DialogPointOfIntersection : public DialogTool
{
    Q_OBJECT
public:
                   /**
                    * @brief DialogPointOfIntersection create dialog
                    * @param data container with data
                    * @param parent parent widget
                    */
                   DialogPointOfIntersection(const VContainer *data, QWidget *parent = 0);
                   ~DialogPointOfIntersection();
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
     * @brief getFirstPointId return id of first point
     * @return id
     */
    inline qint64  getFirstPointId() const {return firstPointId;}
    /**
     * @brief setFirstPointId set id of first point 
     * @param value id
     * @param id don't show this id in list.
     */
    void           setFirstPointId(const qint64 &value, const qint64 &id);
    /**
     * @brief getSecondPointId return id of second point
     * @return id
     */
    inline qint64  getSecondPointId() const {return secondPointId;}
    /**
     * @brief setSecondPointId set id of second point
     * @param value id
     * @param id don't show this id in list.
     */
    void           setSecondPointId(const qint64 &value, const qint64 &id);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void   ChoosedObject(qint64 id, const Scene::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void   DialogAccepted();
private:
    Q_DISABLE_COPY(DialogPointOfIntersection)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogPointOfIntersection *ui;
    /**
     * @brief number number of handled objects
     */
    qint32         number;
    /**
     * @brief pointName name of point
     */
    QString        pointName;
    /**
     * @brief firstPointId id first point of line
     */
    qint64         firstPointId;
    /**
     * @brief secondPointId id second point of line
     */
    qint64         secondPointId;
};

#endif // DIALOGPOINTOFINTERSECTION_H

/************************************************************************
 **
 **  @file   dialogdetail.h
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

#ifndef DIALOGDETAIL_H
#define DIALOGDETAIL_H

#include "ui_dialogdetail.h"
#include "dialogtool.h"

/**
 * @brief The DialogDetail class dialog for ToolDetai. Help create detail and edit option.
 */
class DialogDetail : public DialogTool
{
    Q_OBJECT
public:
                     /**
                      * @brief DialogDetail create dialog
                      * @param data container with data
                      * @param parent parent widget
                      */
                     DialogDetail(const VContainer *data, QWidget *parent = nullptr);
    /**
     * @brief getDetails return detail
     * @return detail
     */
    VDetail          getDetails() const;
    /**
     * @brief setDetails set detail
     * @param value detail
     */
    void             setDetails(const VDetail &value);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of objects (points, arcs, splines, spline paths)
     * @param type type of object
     */
    virtual void     ChoosedObject(quint32 id, const Valentina::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void     DialogAccepted();
    /** TODO ISSUE 79 : create real function
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void      DialogApply(){}
    /**
     * @brief BiasXChanged changed value of offset for object respect to x
     * @param d value in mm
     */
    void             BiasXChanged(qreal d);
    /**
     * @brief BiasYChanged changed value of offset for object respect to y
     * @param d value in mm
     */
    void             BiasYChanged(qreal d);
    /**
     * @brief ClickedSeams save supplement of seams for detail
     * @param checked 1 - need supplement, 0 - don't need supplement
     */
    void             ClickedSeams(bool checked);
    /**
     * @brief ClickedClosed save closed equdistant or not
     * @param checked 1 - closed, 0 - don't closed
     */
    void             ClickedClosed(bool checked);
    /**
     * @brief ObjectChanged changed new object (point, arc, spline or spline path) form list
     * @param row number of row
     */
    void             ObjectChanged(int row);
    /**
     * @brief DeleteItem delete item from list
     */
    void             DeleteItem();
private:
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogDetail ui;
    /**
     * @brief details detail
     */
    VDetail          details;
    /**
     * @brief supplement keep option supplement of seams
     */
    bool             supplement;
    /**
     * @brief closed keep option about equdistant (closed or not)
     */
    bool             closed;
    /**
     * @brief NewItem add new object (point, arc, spline or spline path) to list
     * @param id id of object
     * @param typeTool type of tool
     * @param mode mode
     * @param typeNode type of node in detail
     * @param mx offset respect to x
     * @param my offset respect to y
     */
    void             NewItem(quint32 id, const Valentina::Tools &typeTool, const NodeDetail::NodeDetails &typeNode,
                             qreal mx = 0, qreal my = 0);
};

inline VDetail DialogDetail::getDetails() const
{
    return details;
}

#endif // DIALOGDETAIL_H

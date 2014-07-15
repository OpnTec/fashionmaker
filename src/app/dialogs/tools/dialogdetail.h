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
#include "../../geometry/vdetail.h"

/**
 * @brief The DialogDetail class dialog for ToolDetai. Help create detail and edit option.
 */
class DialogDetail : public DialogTool
{
    Q_OBJECT
public:
    DialogDetail(const VContainer *data, QWidget *parent = nullptr);

    VDetail          getDetails() const;
    void             setDetails(const VDetail &value);
public slots:
    virtual void     ChoosedObject(quint32 id, const SceneObject &type);
    virtual void     DialogAccepted();
    /** TODO ISSUE 79 : create real function
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void     DialogApply(){}
    void             BiasXChanged(qreal d);
    void             BiasYChanged(qreal d);
    void             ClickedSeams(bool checked);
    void             ClickedClosed(bool checked);
    void             ObjectChanged(int row);
    void             DeleteItem();
    virtual void     UpdateList();
private:

    /** @brief ui keeps information about user interface */
    Ui::DialogDetail ui;

    /** @brief details detail */
    VDetail          details;

    /** @brief supplement keep option supplement of seams */
    bool             supplement;

    /** @brief closed keep option about equdistant (closed or not) */
    bool             closed;

    void             NewItem(quint32 id, const Tool &typeTool, const NodeDetail &typeNode,
                             qreal mx = 0, qreal my = 0);
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getDetails return detail
 * @return detail
 */
inline VDetail DialogDetail::getDetails() const
{
    return details;
}

#endif // DIALOGDETAIL_H
